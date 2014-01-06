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

Wie bereits in einer früheren Aufgabe, soll ein Treiber geschrieben werden. Dieser soll einen eigenen Puffer besitzen auf den lesend und schreibend zugegriffen werden kann. Der Zugriff soll nun aber über Threads geschehen.

Wir wollen damit den Zugriff auf einen Puffer auf einer externen Hardware, die über den Treiber angesprochen wird, simulieren. Über die Threads lässt sich der Lese-/Schreibzugriff asynchron realisieren. 

###1.1 Vorgaben
Der zu benutzende Speicher soll dynamisch mit kmalloc() allokiert werden. 
Das schreiben und Lesen des Puffers soll nun ausschließlich über Kernel-Threads realisiert werden.
Kritische Bereiche sollen, wenn möglich, vermieden werden.
Es darf keine Race-Conditions geben.
Der Schreibthread soll eine höhere Priorität als der LeseThread haben.
Simulieren von langsamer Hardware mit der sleep() Funktion.

##2.  Lösung

###2.1 Puffer

Der Puffer wird mithilfe eines Listen Structs implementiert.
Er enthält neben dem Pointer auf den nächsten Eintrag, auch einen Pointer auf den allokierten Speicherbereich.

struct liste 
{ 
  liste *next;
  void *value;
};

###2.2 Funktionen
Bei jedem Lesenden oder Schreibenden Zugriff wird ein neuer Thread erstellt. Das erfordert das Erstellen von zwei neuen Funktionen, welche dann von den Schreib- und Lesethreads ausgeführt werden. 

####2.2.1 list_add(void* pContent)
Fügt der Liste am Ende den Pointer zum neuen dynamischen Speicherbereich hinzu und allokiert Speicherbereich für das nächste Listenelement. 
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

###2.3 Schutz
Die Kritischen Bereiche des Puffers werden in "readthread()" und "writethread()" mit einem Mutex geschützt. Im kritischen Bereich befinden sich nur der Zugriff auf die Puffer allokation der Liste. 

###2.3 Parametrisieren 
Der Puffer soll zudem Parametrisierbar sein.
Darunter verstehen wir das Beschränken der Puffergröße auf einen bestimmten Wert. Idealerweise sollte dieser an das ausführende System angepasst sein.

####2.3.1 Parametrisieren beim Laden des Moduls
Entweder man setzt von Anfang an ein festes Limit, welches nicht überschritten werden darf, oder der Verfügbare Speicher wird bei der Initialisierung des Treibers abgefragt und das Limit so berechnet.  
Vor jedem schreiben in den dynamischen Puffer muss dabei aber überprüft werden, ob das Limit überschritten wird. Wenn ja, darf nicht geschrieben werden!

####2.3.2 Parametrisieren zur Laufzeit
Man könnte die Größe des noch verfügbaren Arbeitsspeichers vor jedem allokieren von Speicher abfragen und dann entscheiden ob der Puffer weiterhin erweitert werden darf.

##3. Weitere Probleme/Themen
Ein return "0" in der buf_write Funktion führte zum endlosen wiederholten aufrufen von Write, bis der Threadvorrat erschöpft war.

Beim wiederholten schreiben in den Puffer kam es zu einem Problem, der den Treiber crashen ließ.
Es ist von der Aufgabenstellung nicht ganz klar wie der Puffer den genau auszusehen hat. Da das aber nicht ausschlaggebend beschlossen wir eine Liste zu benutzen. 

Beim übergeben der Werte an Write kam es zum speichern falscher Werte / des falschen Speicherbereichs

##4. Kritik

##5. Milestones
