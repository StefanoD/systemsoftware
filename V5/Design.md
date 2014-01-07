##Inhalt
1. Aufgabenstellung
 1. Vorgaben
2. Lösung
 1. Puffer
  1. Einleitung
  2. Funktionen
   1. liste_add()
   2. liste_remove()
   3. liste_clear()
   4. buf_read()
   5. buf_write()
   6. readthread()
   7. writethread()
3. Weitere Probleme/Themen
4. Kritik
5. Milestones



##1. Aufgabenstellung

Wie bereits in einer früheren Aufgabe, ist das Ziel die Programmierung eines Treibers. Dieser soll einen eigenen Puffer besitzen, auf den lesend und schreibend zugegriffen werden kann. Anders als bisher wird der Speicher für den Puffer nun aber dynamisch reserviert. Der Zugriff darauf soll nun außerdem asynchron über Threads geschehen.

Das Ziel des ganzen ist es, einen Zugriff auf eine externe Hardware zu simulieren.

###1.1 Vorgaben
* Der zu benutzende Speicher soll dynamisch mit kmalloc() reserviert werden. 
* Das schreiben und Lesen des Puffers soll nun ausschließlich über Kernel-Threads realisiert werden.
* Kritische Bereiche sollen, wenn möglich, vermieden werden.
* Es darf keine Race-Conditions geben.
* Der Schreibthread soll eine höhere Priorität als der Lesethread haben.
* Die Langsame Hardware soll mithilfe eines "random sleep" in den jeweiligen Threads simuliert werden.

##2.  Lösung

###2.1 Puffer

Der Puffer wird mithilfe eines Listen-Structs implementiert.
Das struct enthält einen Pointer auf das nächste Listenelement und einen Pointer auf den allokierten Speicherbereich.
Dieser wird dann per memcopy() "befüllt".

    struct liste 
    { 
      liste *next;
      void *value;
    };

###2.2 Funktionen
Bei jedem Lesenden oder Schreibenden Zugriff wird ein neuer Thread erstellt. Das erfordert das implementieren von zwei neuen Funktionen, welche dann von den Schreib- und Lesethreads ausgeführt werden. 

####2.2.1 list_add(char *pContent)
Diese Funktion reserviert dynamisch genug Speicher für den Inhalt des übergebenen Pointers und kopiert diesen dann in den Speicherbereich. Hierfür wird zuerst kmalloc() mit dem GFP_ATOMIC Flag benutzt.
Zum kopieren der Daten in den neu allokierten Bereich wird anschließend memcpy() aufgerufen.

Diese Funktion wird von den Schreibenden Threads aufgerufen.

####2.2.2 liste_remove()
Entfernt den Letzten Eintrag der Liste und gibt ihn zurück. 
Diese Funktion wird von den Lesenden Threads aufgerufen.

####2.2.3 liste_clear()
Befreit den Speicherbereich von sämtlichen noch in der Liste vorhandenen Elementen und der Liste selbst und löscht so die Komplette Liste. 

####2.2.4 buf_read()
Diese Funktion wird aufgerufen, wenn lesend auf den Treiber zugegriffen wird. z.B. mit "cat /dev/buf"
Es wird ein neuer Lesethread erstellt. Dieser liest den neuesten Eintrag der Liste, sowie die zu speichernden Daten übergeben.
Dann wird der neue Thread gestartet.

####2.2.5 buf_write()
Diese Funktion wird aufgerufen, wenn schreibend auf den Treiber zugegriffen wird. z.B. mit "echo "irgend ein Text" > /dev/buf"
Es wird ein neuer Schreibthread erstellt. Dieser bekommt die writethread Funktion, sowie die zu speichernden Daten übergeben.
Dann wird der neue Thread gestartet.

####2.2.6 readthread()
Der Thread der auf den Puffer lesend zugreift.

####2.2.7 writethread()
Der Schreibende Thread.

###2.3 Parametrisieren 
Der Puffer soll zudem Parametrisierbar sein.
Darunter verstehen wir das Beschränken der Puffergröße auf einen bestimmten Wert. Idealerweise sollte dieser an das ausführende System angepasst sein.

####2.3.1 Parametrisieren beim Laden des Moduls
Entweder man setzt von Anfang an ein festes Limit, welches nicht überschritten werden darf, oder der Verfügbare Speicher wird bei der Initialisierung des Treibers abgefragt und das Limit so berechnet.  
Vor jedem schreiben in den dynamischen Puffer muss dabei aber überprüft werden, ob das Limit überschritten wird. Wenn ja, darf nicht geschrieben werden!

####2.3.2 Parametrisieren zur Laufzeit
Man könnte die Größe des noch verfügbaren Arbeitsspeichers vor jedem allokieren von Speicher abfragen und dann entscheiden ob der Puffer weiterhin erweitert werden darf.

####2.4 Datenfluss

    Schreiben:
    echo "abc" > /dev/buf  ---->  buf_write() ----> create and start writethread() ----> liste_add() -----
                                   ^                                                                     |
                                   '----------------------------------------------------------------------          

    Lesen:
    cat /dev/buf           ---->  buf_read() ----> create and start readthread() ----> liste_remove() ---- 
                                   ^                                                                     |
                                   '----------------------------------------------------------------------          

####2.5 Kontrollfluss


###2.3 Kritische Bereiche
Die Kritischen Bereiche des Puffers werden in "readthread()" und "writethread()" mit einem Mutex geschützt. Im kritischen Bereich befinden sich nur der Zugriff auf die Puffer allokation der Liste. 




##3. Weitere Probleme/Themen
Ein return "0" in der buf_write Funktion führte zum endlosen wiederholten aufrufen von Write, bis der Threadvorrat erschöpft war.

Beim wiederholten schreiben in den Puffer kam es zu einem Problem, der den Treiber crashen ließ. Durch Zufall funktionierte ab und zu der Zugriff auf einen nicht von uns reservierten Speicherbereich. Der Fehler viel deswegen nicht so schnell auf.

Es ist von der Aufgabenstellung nicht ganz klar wie der Puffer den genau auszusehen hat. Da das aber nicht ausschlaggebend ist, beschlossen wir eine Liste zu benutzen. 

##4. Kritik
Da das Thema recht aufwendig ist und somit viel Zeit kostet, wäre es sinnvoll gewesen früher anzufangen. 

##5. Milestones
