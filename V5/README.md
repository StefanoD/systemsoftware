### V5

######Untersuchen Sie Ihre Lösung auf Race Conditions! Wo können Race Conditions auftreten und wie können diese verhindert werden? 
* beim zugriff auf den memoryPointer
* verhindern mit atomic_set und co!


#####Wie sähe die Lösung der Aufgabenstellung über den Schutz des kritischen Bereichs mit Spinlocks aus? Welche Lösung ist vorzuziehen und warum? 
* spinlock_t lock;
* Semaphore, da spinlock busy waiting mit sich bringt!
* Semaphore können auch auf mehrere verschiedene Threads angewandt werden.
