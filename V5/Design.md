# Todo alles ändern da nun mit komplett neuer implementierung YAY!

##Inhalt
1. Aufgabenstellung
 1. Vorgaben
2. Lösung
 1. Puffer
 2. Funktionen
  1. liste_add()
  2. liste_remove()
  3. liste_clear()
  4. buf_read()
  5. buf_write()
  6. readthread()
  7. writethread()
 3. Parametrisieren
  1. Parametrisieren beim Laden des Moduls
  2. Parametrisieren zur Laufzeit
 4. Kontroll- und Datenfluss
 5. Kritische Bereiche
  1. Mögliche Race-Conditions
3. Probleme
4. Kritik

--------------------------------------------------------

##1. Aufgabenstellung
Wie bereits in einer früheren Aufgabe, ist das Ziel die Programmierung eines Treibers. Dieser soll einen eigenen Puffer besitzen, auf den lesend und schreibend zugegriffen werden kann. Anders als bisher wird der Speicher für den Puffer nun aber dynamisch reserviert. Der Zugriff darauf soll nun außerdem asynchron über Threads geschehen.

Das Ziel des ganzen ist es, einen Zugriff auf eine externe Hardware zu simulieren.

###1.1 Vorgaben
* Der zu benutzende Speicher soll dynamisch mit *kmalloc()* reserviert werden. 
* Das schreiben und Lesen des Puffers soll nun ausschließlich über Kernel-Threads realisiert werden.
* Kritische Bereiche sollen, wenn möglich, vermieden werden.
* Es darf keine Race-Conditions geben.
* Der Schreibthread soll eine höhere Priorität als der Lesethread haben.
* Die Langsame Hardware kann mithilfe eines "random sleep" in den jeweiligen Threads simuliert werden.

##2.  Lösung
Nach einigen Rückschlägen bei der Implementierung wurde entschieden, neu anzufangen.

###2.1 Puffer
In der neuen Implementation benutzen wir ein Array, welches mit char Pointern gefüllt wird.
Dies hat den Vorteil, dass einzelne Einträge direkt angesprochen werden können und dass dieses Array von der Größe beschränkbar ist. Dadurch das die Einträge aus Pointern bestehen, wird nicht mehr Speicherplatz reserviert als benötigt! Erst wenn der Speicher wirklich benötigt wird versucht der *writethread()* diesen zu reservieren.
Der einfachhalthaber benutzen wir auch dieses Array wie einen LIFO-Stack.

Der alte Puffer wurde mithilfe eines Listen-Structs implementiert, das auch wie ein Stack benutzt wurde (LIFO)
Das struct enthielt einen Pointer auf das nächste Listenelement und einen Pointer auf den allokierten Speicherbereich.
Dieser wurde dann per *memcopy()* "befüllt". 

    struct liste 
    { 
      liste *next;
      void *value;
    };

Die alte Implementation war viel komplexer und vom Code nicht ausgereift genug. Es gab auch keinerelei Einschränkungen der Datenmengen. 


#TODO hier WEiter

###2.2 Funktionen
Bei jedem Lesenden oder Schreibenden Zugriff wird nach wie vor ein neuer Thread erstellt. Das erfordert das implementieren von zwei neuen Funktionen, welche dann von den Schreib- und Lesethreads ausgeführt werden. 

####2.2.1 buf_read(struct file *instance , char __user *buf, size_t len, loff_t off)
Diese Funktion wird aufgerufen, wenn lesend auf den Treiber zugegriffen wird. z.B. mit "cat /dev/buf"
Es wird bei jedem Aufruf ein neuer Lesethread erstellt. 

Der Thread wird erst mit *kthread_create()* erstellt und dann mit *wake_up_process()* gestartet. 

    thread_id = kthread_create(readthread, NULL, "readthread");
    

####2.2.2 buf_write(struct file *instance , const char _user *buf, size_t len, loff_t off)
Diese Funktion wird aufgerufen, wenn schreibend auf den Treiber zugegriffen wird. 

    echo "irgend ein Text" > /dev/buf"
    
Es wird bei jedem Aufruf ein neuer Schreibthread erstellt. 
Der Thread wird erst mit *kthread_create()* erstellt und dann mit *wake_up_process()* gestartet. 

    thread_id = kthread_create(writethread, (void *) buf, "writethread");
    

####2.2.3 readthread()
Diese von einem Kernelthread ausgeführte Funktion greift lesend auf den Puffer zu.
Hierzu ruft sie  *liste_remove()* auf. 

####2.2.4 writethread()
Diese von einem Kernelthread ausgeführte Funktion greift schreibend auf den Puffer zu.
Hierzu ruft sie  *liste_add()* auf.

###2.3 Parametrisieren 
Der Puffer soll zudem Parametrisierbar sein.
Darunter verstehen wir das Beschränken der Puffergröße auf einen bestimmten Wert. Idealerweise sollte dieser an das ausführende System angepasst sein.

####2.3.1 Parametrisieren beim Laden des Moduls
Entweder man setzt von Anfang an ein festes Limit, welches nicht überschritten werden darf, oder der Verfügbare Speicher wird bei der Initialisierung des Treibers abgefragt und das Limit so berechnet.  
Vor jedem schreiben in den dynamischen Puffer muss dabei aber überprüft werden, ob das Limit überschritten wird. Wenn ja, darf nicht geschrieben werden!

####2.3.2 Parametrisieren zur Laufzeit
Man könnte die Größe des noch verfügbaren Arbeitsspeichers vor jedem allokieren von Speicher abfragen und dann entscheiden ob der Puffer weiterhin erweitert werden darf.

####2.4 Kontroll- und Datenfluss

![Alt Read](/Treiber/img/read.png)
![Alt Write](/Treiber/img/write.png)

###2.5 Kritische Bereiche
Die Kritischen Bereiche des Puffers werden in *readthread()* und *writethread()* mit einem Mutex geschützt. Das ist für unsere Implementierung völlig ausreichend.
Im kritischen Bereich befindet sich  der Zugriff auf die Puffer-Allokation der Liste. 
* Write: Beim Speicher reservieren und kopieren in den Puffer, also um die *liste_add()* Funktion
* Read: Beim lesen aus dem Puffer, also um die *liste_remove* Funktion

Zu weiteren Konflikten kann es zwischen Read- und Write-Instanzen kommen.

####2.5.1 Mögliche Race-Conditions
* Der SchreibeThread im foldenen WT genannt, will gerade schreiben, wird dann schlafen gelegt und ein Lesethread (RT) liest und entfernt somit den Eintrag im Puffer.
* RT war schlafen, wurde geweckt und will gerade lesen,  aber der Puffer wurde schon von einem anderen RT "geleert" 
...

Die Race-Conditions werden mit hilfe der Kernel Makros *wait_event_interruptible()* und *wake_up_interruptible()* verhindert.


##3. Probleme
Auf dem Weg zur letztendlichen Implementierung trafen wir auf einige Probleme: 
* Ein return "0" in der *buf_write()* Funktion führte zum endlosen wiederholten aufrufen von Write, bis der Threadvorrat erschöpft war.
* Beim wiederholten schreiben in den Puffer kam es zu einem Problem, der den Treiber crashen ließ. Durch Zufall funktionierte ab und zu der Zugriff auf einen nicht von uns reservierten Speicherbereich. Der Fehler viel deswegen nicht so schnell auf.
* Es ist von der Aufgabenstellung nicht ganz klar wie der Puffer den genau auszusehen hat. Da das aber nicht ausschlaggebend ist, implementierten wir eine einfache Liste.

##4. Kritik
Da das Thema recht zeitaufwendig ist, wäre es sinnvoll gewesen früher anzufangen. 




###Alte Funktionen
####2.2.1 list_add(char *pContent)
Diese Funktion reserviert dynamisch genug Speicher für den Inhalt des übergebenen Pointers und kopiert diesen dann in den Speicherbereich. Hierfür wird zuerst *kmalloc()* mit dem *GFP_ATOMIC* Flag benutzt.
Zum kopieren der Daten in den neu allokierten Bereich wird anschließend *memcpy()* aufgerufen.
Diese Funktion wird von den Schreibenden Threads aufgerufen.

####2.2.2 liste_remove()
Entfernt den Letzten Eintrag aus der Liste und gibt diesen dann an den aufrufenden Thread zurück.
Beim Entfernen wird außerdem der zugewiesene Speicher befreit.
Diese Funktion wird von den Lesenden Threads aufgerufen.

####2.2.3 liste_clear()
Befreit den kompletten vorher reservierten Speicherbereich der gesammten Liste und aller beinhalteten Elementen.
Diese Funktion wird beim entladen des Treibers aufgerufen.
