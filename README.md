# TODO's
- Hintergrundmaterial und Recherchen
  - [x] Readme mit Grundstruktur anlegen
  - [x] Materialbeschaffung abschließen


- Verwendung von ThreadSanitizer und GoodLock
  - [x] Verwenden von Tsan unter Windows
  - [x] Eigene Beispiele mit Tsan durchführen und beschreiben
  - [x] False Negative und false positives Fälle für TSan überprüfen


- Eigenständige Implementierung in C++
  - [x] Gedanken zur Struktur machen
  - [x] Implementieren
  - [x] Implementierung testen
  - [x] Doc schreiben
  - [ ] Code cleanup


- Allgemeines
  - [ ] Zusammenfassung der gewonnenen Erkenntnisse
  - [ ] Denglisch ausmerzen
  - [ ] Bleiwüste aufräumen
  - [ ] Tippfehler & Grammatikfehler eliminieren
  - [ ] Überschriften überprüfen

# GoodLock-Projektarbeit
## Einführung
Im Rahmen dieser Projektarbeit arbeiten Marie Fiederlein und Fabio Nocera gemeinsam am Thema GoodLock. Betreut wird das Thema von Herrn Prof. Dr. Martin Sulzmann.

GoodLock ist ein Algorithmus, welcher dazu dient Programmverhalten dynamisch zu analysieren und Deadlocks zu erkennen. Das Ziel ist es sich mit dem Algorithmus, der Funktionsweise, dem Einsatz und einer eigenen Implementierung zu beschäftigen.

Hierbei ist der schriftliche Teil in drei Teile untergliedert:

1. Hintergrundmaterial und Recherchen

2. Verwendung von ThreadSanitizer und GoodLock

3. Eigenständige Implementierung in C++

# Hintergrundmaterial und Recherchen
## Wissenschaftliche Arbeiten/Paper
- Der Algorithmus für die Detektion von Deadlocks und data Races mit maximal 2 Threads wird erklärt und in Java implementiert.
   - [Using Runtime Analysis to Guide Model Checking of Java Programs](<https://ti.arc.nasa.gov/m/pub-archive/177h/0177%20(Havelund).pdf>)
- Der Algorithmus für die Detektion von Deadlocks mit n Threads wird erklärt und in Java implementiert.
   - [Detecting Potential Deadlocks with Static Analysis and Run-Time Monitoring](https://www.research.ibm.com/haifa/Workshops/PADTAD2005/papers/article.pdf)
- Der Algorithmus des ThreadSanitizer wird erklärt und seine Einsatzgebiete vom Testen von multithreaded C++ Code aufgezeigt.
   - [ThreadSanitizer – data race detection in practice](https://static.googleusercontent.com/media/research.google.com/de//pubs/archive/35604.pdf)
- Es werden zwei Analyse Tools, ThreadSanitizer und ThreadSafety, die zum Testen von multithreaded Programmen genutzt werden, näher betrachtet und deren Vor- und Nachteile aufgezeigt.
   - [How Developers Use Data Race Detection Tools](https://static.googleusercontent.com/media/research.google.com/en//pubs/archive/43217.pdf)

## Beispiele
- Eine Beschreibung einiger der häufigsten Daten Races, die vom ThreadSanitizer erkannt werden.
   - [ThreadSanitizerPopularDataRaces](https://github.com/google/sanitizers/wiki/ThreadSanitizerPopularDataRaces)
- False Positives:
   - [ThreadSanitizer gives false positive warning about Deadlock](https://gist.github.com/spetrunia/77274cf2d5848e0a7e090d622695ed4e)
   - [Thread Sanitizer false positive on lock-order-inversion (potential Deadlock)](https://github.com/google/sanitizers/issues/1419)
   - [False lock-order-inversion reports for locks taken in a single thread](https://github.com/google/sanitizers/issues/488)
   - [tsan: False positive for lock-order-inversion for lock cycles with asymmetrical unlocking](https://github.com/google/sanitizers/issues/814)
- False Negatives:
   - [Zwei Threads in Abhängigkeit](https://sulzmann.github.io/AutonomeSysteme/lec-deadlock.html#(3))
   - [Weitere häufige false Negatives werden beschrieben](https://groups.google.com/g/thread-sanitizer/c/mB73m6Nltaw)

## Implementierungen
- Implementierung verschiedener Sanitizer unter anderem den ThreadSanitizer in den Sprachen C++ und Go. Die zugrundeliegende Implementierung vom ThreadSanitizer ist aus dem LLVM-Project. Es wird sehr grob beschrieben, wie Tsan funktioniert.
   - [google/sanitizers](https://github.com/google/sanitizers)
   - [google/sanitizers/wiki/ThreadSanitizerAlgorithm](https://github.com/google/sanitizers/wiki/ThreadSanitizerAlgorithm)
- Die grundlegende Implementierung von ThreadSanitizer in C++ von LLVM. Die LLVM Compiler Infrastructure ist eine Sammlung von modularen und wiederverwendbaren Compilern und toolchain-Techniken.
   - [Docs](https://clang.llvm.org/docs/ThreadSanitizer.html)
   - [Codebase](https://github.com/llvm/llvm-project/tree/main/compiler-rt/lib/tsan)
- Tests aus dem Repository von LLVM.
   - [llvm-project/compiler-rt/test/tsan](https://github.com/llvm/llvm-project/tree/main/compiler-rt/test/tsan)

## Diskussionen und Erklärungen
- Es wird über den Speicherplatzbedarf für das Speichern von geteilten Variablen und/oder Zugriffen des gemeinsamen Lesens von TSan diskutiert und wie das Löschen beim Überlauf gelöst ist.
   - [Speicherverwaltung von Tsan für seine gespeicherten Variablen](https://groups.google.com/g/thread-sanitizer/c/mB73m6Nltaw)
- TSan entdeckt nur potenzielle Deadlocks. Falls tatsächlich ein Deadlock während des Tests auftritt, dann wird sich TSan aufhängen.
   - [Verhalten von TSan bei einem auftretenden Deadlock](https://groups.google.com/g/thread-sanitizer/c/g3aMe3IxVBQ)
- Es werden Videos und Präsentationen angegeben, um mehr Details zur Funktionalität von TSan zu erfahren.
   - [Erklärungen zur Funktionalität von TSan](https://groups.google.com/g/thread-sanitizer/c/HquBSCbzbyA)
- Talk über die Funktion des Go race detectors, welcher auf der Tsan Implementierung in LLVM basiert. Es wird auch die Funktionsweise von Tsan selbst erläutert.
   - [Looking inside a Race Detector](https://www.infoq.com/presentations/go-race-detector/)
- Präsentation über ThreadSanitizer v2.
   - [2012 LLVM Developers’ Meeting: K. Serebryany “MemorySanitizer, ThreadSanitizer. Scalable ...”](https://www.youtube.com/watch?v=HDgttiIvMxA)
   - [Slides](https://llvm.org/devmtg/2012-11/Serebryany_TSan-MSan.pdf)
- Der ThreadSanitizer v1 basiert auf Valgrind/Helgrind basiert. Das ist eine kleine Übersicht zu Helgrind.
   - [Helgrind: a thread error detector](https://valgrind.org/docs/manual/hg-manual.html#hg-manual.lock-orders)
- Präsentation darüber, warum Tsan und andere Sanitizer nicht ausreichend sind. Wenn die Sanitizer Bugs finden ist das schön, sie liefern allerdings keinen Beweis der Korrektheit. Es wird zusätzlich Fuzzing zum code hardening empfohlen.
   - [2015 LLVM Developers’ Meeting: K. Serebryany & P. Collingbourne "Beyond Sanitizers..."](https://www.youtube.com/watch?v=5K_uIda0tZU)

# Verwendung von ThreadSanitizer und dessen GoodLock Implementierung
## Einführung
Der ThreadSanitizer ist Teil des [LLVM Projects](https://llvm.org/), einer Sammlung von modularen und wiederverwendbaren compiler und toolchain Technologien sowie Teil von [gcc](https://gcc.gnu.org/) und [clang](https://clang.llvm.org/).
Insbesondere ist der ThreadSanitizer Teil der Codebase von [compiler-rt](https://compiler-rt.llvm.org/) zusammen mit anderen Sanitizern, wie zum Beispiel der [AdressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html).
ThreadSanitizer übernimmt hier die Funktion eines Tools, welches [data races](https://docs.oracle.com/cd/E19205-01/820-0619/geojs/index.html) sowie [DeadLocks](https://docs.oracle.com/cd/E19205-01/820-0619/geokj/index.html) erkennt.
Je nach Situation ist ein slowdown von 5x-15x sowie ein Speicher overhead von 5x-10x zu erwarten.

Bis heute ist ThreadSanitizer in der Betaphase, insgesamt aber dafür bekannt zuverlässig in großen C++ Projekten unter Verwendung von pthreads zu funktionieren.

## Problematiken
Der oben genannte slowdown und overhead kann je nach Anwendung kritisch sein, es ist zu empfehlen dies vorher zu prüfen.

Tsan bietet keinerlei Garantien dafür, dass data races gefunden werden und gefundene data races liefern keinen Beweis der Korrektheit ([K. Serebryany, Google](https://www.youtube.com/watch?v=5K_uIda0tZU)).
Trotz, dass der ThreadSanitizer mittlerweile ausgereift und häufig verwendet wird, ist er nach wie vor in der Betaphase, erfährt jedoch aktive Entwicklung.
Die Deadlock Detection wird [Stand 2020, K. Serebryany, Google](https://github.com/google/sanitizers/issues/1258) seit circa 2015 leider nicht aktiv weiterentwickelt.
Darüber hinaus gibt es auch zwei Versionen im Repository [Deadlock_detector1](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/sanitizer_common/sanitizer_deadlock_detector1.cpp) und [Deadlock_detector2](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/sanitizer_common/sanitizer_Deadlock_detector2.cpp).
Diese liefern unterschiedliche Ergebnisse, insgesamt ist die [Deadlock Erkennung als Experimental](https://github.com/google/sanitizers/wiki/ThreadSanitizerDeadlockDetector) eingestuft.
Inwiefern diese Funktionalität im Vergleich zu der data race Erkennung des Tsan eine Anwendung in der Industrie erfährt, konnte leider nicht in Erfahrung gebracht werden.
Es ist außerdem nicht bekannt in welchem Rahmen die Deadlock Erkennung funktional ist, das lässt sich aus mehreren Stellen des LLVM repositories ableiten.
[Insbesondere können keine Deadlocks erkannt werden, die wirklich passieren](https://github.com/llvm/llvm-project/blob/main/compiler-rt/test/tsan/must_deadlock.cpp). Das liegt daran, dass die Überprüfung des Graphen und damit die Erkennung des Deadlocks nach pthread_mutex_lock ausgeführt wird und sich das Programm entsprechend schon im Deadlock befindet.

## Unterstütze Plattformen und Datentypen
### Plattformen

- Android aarch64, x86_64
- Darwin arm64, x86_64
- FreeBSD
- Linux aarch64, x86_64, powerpc64, powerpc64le
- NetBSD

### Datentypen
- ```pthread_mutex_*```
- ```pthread_rwlock_*```
- ```pthread_spin_*```

Anmerkung: Es wird std::thread verwendet. Dieser library liegt pthread zugrunde.

## Einrichtung unter Windows
Voraussetzung für die Anwendung von Tsan unter Windows ist [WSL/WSL2](https://docs.microsoft.com/de-de/windows/wsl/compare-versions) mit einer der unterstützten [Plattformen](#Plattformen) als Subsystem.
Gewählt wurde in unserem Fall WSL2 mit Ubuntu.

In Ubuntu ist es dann ausreichend ein paar Befehle im Terminal auszuführen.
1. ```sudo apt-get update``` sowie ```sudo apt-get upgrade``` um das System zu aktualisieren.
2. ```sudo apt-get install cmake``` zum installieren von [cmake](https://cmake.org/).
3. ```sudo apt-get install gdb``` zum installieren des [gdb](https://www.gnu.org/software/gdb/) debuggers.
4. ```sudo apt-get install build-essential``` um den [gcc compiler](https://gcc.gnu.org/) zu installieren.

Damit ist seitens Ubuntu alles eingerichtet. Um CLion dazu zu bringen, auf dem Subsystem zu kompilieren, kann man die von [JetBrains beschriebenen Schritte](https://www.jetbrains.com/help/clion/how-to-use-wsl-development-environment-in-product.html#wsl-tooclhain) befolgen.
Jetzt ist es noch nötig CMake mitzuteilen, dass wir mit aktiven ThreadSanitizer kompilieren möchten. Das geht, indem man die folgende Zeile zu [CMakeLists](CMakeLists.txt) hinzufügt:
```set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -g -O2")```.
Hierbei generiert ```-g``` [debug Informationen](https://www.rapidtables.com/code/linux/gcc/gcc-g.html) für den GDB debugger und ```-O2``` für den [Optimierungsgrad](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html).
Zum Schluss übergeben wir noch zwei Flags an Tsan.
- ```detect_deadlocks=1``` um die Deadlock Erkennung zu aktivieren.
- ```second_deadlock_stack=1``` um erweitere Informationsausgabe auf der Konsole zu aktivieren.

Es ist nun möglich mit Tsan zu kompilieren.

## Ein Beispiel mit pthreads
Das folgende Beispiel erzeugt einen für ThreadSanitizer potenziellen Deadlock mit einem pthread.

```c++
void test_simple_pthread_example() {
    pthread_mutex_t px, py;
    pthread_mutex_init(&px, NULL);
    pthread_mutex_init(&py, NULL);
    
    pthread_mutex_lock(&px);
    pthread_mutex_lock(&py);
    pthread_mutex_unlock(&py);
    pthread_mutex_unlock(&px);
    
    pthread_mutex_lock(&py);
    pthread_mutex_lock(&px);
    pthread_mutex_unlock(&px);
    pthread_mutex_unlock(&py);

    pthread_mutex_destroy(&px);
    pthread_mutex_destroy(&py);
}
```

Die Konsolenausgabe zeigt uns ebendies an:

```
==================
WARNING: ThreadSanitizer: lock-order-inversion (potential deadlock) (pid=1714)
  Cycle in lock order graph: M12 (0x7ffd4be1c970) => M13 (0x7ffd4be1c9a0) => M12

  Mutex M13 acquired here while holding mutex M12 in main thread:
    #0 pthread_mutex_lock ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:4165 (libtsan.so.0+0x526fc)
    #1 test_simple_pthread_example() /mnt/c/Users/marie/CLionProjects/Goodlock-Projektarbeit/tests/test_tsan_deadlock_detection.cpp:72 (GoodLock_Projektarbeit+0x3200)
    #2 main /mnt/c/Users/marie/CLionProjects/Goodlock-Projektarbeit/tests/test_tsan_deadlock_detection.cpp:46 (GoodLock_Projektarbeit+0x29c6)

  Mutex M12 previously acquired by the same thread here:
    #0 pthread_mutex_lock ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:4165 (libtsan.so.0+0x526fc)
    #1 test_simple_pthread_example() /mnt/c/Users/marie/CLionProjects/Goodlock-Projektarbeit/tests/test_tsan_deadlock_detection.cpp:71 (GoodLock_Projektarbeit+0x31f8)
    #2 main /mnt/c/Users/marie/CLionProjects/Goodlock-Projektarbeit/tests/test_tsan_deadlock_detection.cpp:46 (GoodLock_Projektarbeit+0x29c6)

  Mutex M12 acquired here while holding mutex M13 in main thread:
    #0 pthread_mutex_lock ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:4165 (libtsan.so.0+0x526fc)
    #1 test_simple_pthread_example() /mnt/c/Users/marie/CLionProjects/Goodlock-Projektarbeit/tests/test_tsan_deadlock_detection.cpp:77 (GoodLock_Projektarbeit+0x3220)
    #2 main /mnt/c/Users/marie/CLionProjects/Goodlock-Projektarbeit/tests/test_tsan_deadlock_detection.cpp:46 (GoodLock_Projektarbeit+0x29c6)

  Mutex M13 previously acquired by the same thread here:
    #0 pthread_mutex_lock ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:4165 (libtsan.so.0+0x526fc)
    #1 test_simple_pthread_example() /mnt/c/Users/marie/CLionProjects/Goodlock-Projektarbeit/tests/test_tsan_deadlock_detection.cpp:76 (GoodLock_Projektarbeit+0x3218)
    #2 main /mnt/c/Users/marie/CLionProjects/Goodlock-Projektarbeit/tests/test_tsan_deadlock_detection.cpp:46 (GoodLock_Projektarbeit+0x29c6)

SUMMARY: ThreadSanitizer: lock-order-inversion (potential deadlock) /mnt/c/Users/marie/CLionProjects/Goodlock-Projektarbeit/tests/test_tsan_deadlock_detection.cpp:72 in test_simple_pthread_example()
==================
ThreadSanitizer: reported 1 warnings

Process finished with exit code 66
```

## Eigenständige Implementierung in C++
### Implementierung
Die grundlegende Implementierung ist in C++ und besteht aus einem LockGraph, der jedes Mutex, mithilfe vom lockSet und dem Array edge, abspeichert.

Mit den Methoden acquire und release werden Mutexe von MyMutex gelockt, wieder unlocked und der LockGraph aktualisiert.
Das Array edge enthält alle Kanten/ Verbindungen zwischen den Mutexen. Beim Initialisieren sind alle Einträge 0, da es noch keine Kanten gibt.
Bei jedem Locken von einem Mutex über ein anderes Mutex, wird die Kante im Array edge gesetzt.

Zum Beispiel: Mutex 0 lockt Mutex 1, Kante in der Zeile 0 und in der Spalte 1 wird auf 1 gesetzt.

Es wird nicht beachtet, welcher Thread welche Kante setzt.

Beim Überprüfen, ob ein Zyklus existiert, wird mithilfe von der Funktion Check über jedes Mutex m iteriert und es werden mithilfe der CheckCycle Methode seine direkten und indirekten Nachbarn angeschaut.
Dabei wird über das Array edges in der Zeile m iteriert und die entsprechenden Mutexe bei den der Eintrag 1 war in einem neuen Set gespeichert.
Für jedes eingetragene Mutex mi im neuen Set wird auch wieder über das Array edges in Zeile mi iteriert und die direkten Nachbarn werden, sofern sie nicht schon besucht worden sind, ins neue Set eingetragen.

Falls wir über die Nachbarn wieder auf das eingesetzte Mutex m kommen, haben wir einen Zyklus und geben eine Meldung aus.
Am Ende wird mit der Funktion info noch angezeigt, welche Mutexe eine gemeinsame Kante besitzen.
### Struktur
#### LockGraph
Die Klasse [LockGraph](src/LockGraph.h) enthält alle Methoden, um die Mutexe vom Typ MyMutex zu acquiren und zu releasen und somit den Lockgraphen zu aktualisieren.

Bei der Methode acquire wird das Mutex gelockt, das zum Thread entsprechende MySet im lockSet aktualisiert.
Falls dieses Mutex über ein anderes Mutex acquired wird, dann wird in edge die Kante gesetzt.
Bei der Methode release wird das Mutex unlocked, das zum Thread entsprechende MySet im lockSet aktualisiert.
Mithilfe der check Methode, kann der Graph auf Zyklen überprüft werden und mit der Methode info, wird der Graph ausgegeben.

Der LockGraph besteht aus einem lockSet, einem Array mutexes, einem zwei dimensionalen Array edge und einem Mutex g.
##### lockSet
Das lockSet ist eine Map mit der Thread ID als Schlüsselvariable und als dazugehöriger Wert ein Set vom Typ MySet.
Es enthält alle Threads und deren MySets genau einmal.
Dieses lockSet enthält die Informationen, welcher Thread welches Mutex in dem Moment acquired hat. 
##### mutexes
Das Array mutexes hat die Länge Max_Mutex und ist vom Typ MyMutex.
In diesem Array werden die initialisierten Mutexe gespeichert.
##### edge
In diesem zwei dimensionalen Array der Größe Max_Mutex * Max_Mutex werden die Kanten zwischen den Mutexen gespeichert.
Es wird mit false initialisiert.
Jedes Mal, wenn ein Mutex m2 über ein anderes Mutex m1 acquired wird, wird in diesem Array in der Zeile m1 in der Spalte m2 die Kante auf wahr gesetzt. 
##### Mutex g
Es handelt sich hier um ein standard Mutex und dient uns, wenn wir über den Graphen iterieren und etwas auslesen, dass kein anderer Thread Zugriff auf den Graphen hat.
So wird ausgeschlossen, dass beim Auslesen falsche Werte gelesen werden.
#### MySet
Die Klasse [MySet](src/MySet.h) enthält eine Map der Länge Max_Mutex und Funktionen, um diese Map zu aktualisieren.
Es enthält eine Map mit der Mutex ID als Schlüsselvariablen und als Wert ein Boolean.
Diese Map wird zuerst mit allen Einträgen auf false initialisiert.

Falls der dazugehörige Thread ein Mutex vom Typ MyMutex acquired, so wird der entsprechende Eintrag in dieser Map auf true gesetzt.
Wird dieses Mutex dann wieder released, so wird der entsprechende Eintrag wieder auf false gesetzt.
Mit der Methode unionSet werden zwei Sets vereinigt und diese Methode wird beim Überprüfen von Zyklen benutzt.
#### MyMutex
Diese Klasse [MyMutex](src/MyMutex.h) stellt uns zu einem Standard Mutex eine Mutex ID zur Verfügung.
#### MyThread
Diese Klasse [MyThread](src/MyThread.h) stellt uns zu einem Standard Thread eine Thread ID zur Verfügung.

### Fälle, die bei dieser Implementierung nicht abgedeckt sind
Die Fälle sind unter anderem in der [Testklasse](tests/test_my_deadlock_detection.cpp) implementiert und ergeben die gleichen falschen Fälle wie beim TSan.\
#### Notation
  | Ausdruck | Bedeutung
  |:---------|:-------|
  | Lock: 0 -> 1| Zuerst wird das Mutex 0 und anschließend Mutex 1 vom aufrufenden Thread gelocked
  | Unlock: 1 -> 0 | Zuerst wird das Mutex 1 und anschließend das Mutex 0 vom aufrufenden Thread entlocked
  | Lock: 0 -> 1 -> 2 | Zuerst wird das Mutex 0, dann das Mutex 1 und dann das Mutex 2 vom aufrufenden Thread gelocked
#### false positive
- Die Implementierung beachtet nicht, welcher Thread, welches Mutex aufruft.\
  So wird bei einem Aufruf mit einem Thread bei 2 Aufrufen in umgekehrter Lockreihenfolge mit zwei Mutexen auch eine Warnmeldung ausgegeben.\
  Obwohl dieser Zyklus mit einem Thread zu keinem Problem führt.

  | Aufruf | Lock | Unlock |
  |:-------------------:|:------------:|:----------------:|
  Erster Aufruf | 0 -> 1 | 1 -> 0
  Zweiter Aufruf | 1 -> 0 | 0 -> 1

- Die Implementierung beachtet die Zeitabstände nicht.\
  So wird bei Aufrufen mit zwei Threads, die mit einer Sekunde Verzögerung zwei Mutexe in umgekehrter Lockreihenfolge locken wollen, eine Warnmeldung ausgegeben.\
  Obwohl dieser Zyklus durch die Zeitverzögerung nicht möglich ist.
  
  | Aufruf | Lock | Unlock |
  |:-------------------:|:------------:|:----------------:|
  Aufruf mit Thread 0: | 0 -> 1 | 1 -> 0
  Thread 1 schläft| | |
  Aufruf mit Thread 1: | 1 -> 0 | 0 -> 1

- Die Implementierung beachtet nicht, ob ein Zyklus tatsächlich zustande kommen kann.\
  So wird bei zwei Aufrufen mit zwei Threads, die beide zuerst Mutex 0 locken und danach zwei andere Mutexe in umgekehrter Lockreihenfolge locken, eine Warnmeldung ausgegeben.\
  Obwohl dieser Zyklus nicht möglich ist, da Thread 2 auf Thread 1 warten wird, bis dieser Mutex 0 wieder freigibt, bevor er andere Mutexe lockt.
  
  | Aufruf | Lock | Unlock |
  |:-------------------:|:------------:|:----------------:|
  Aufruf mit Thread 0: | 0 -> 1 -> 2 | 2 -> 1 -> 0
  Aufruf mit Thread 1: | 0 -> 2 -> 1 | 1 -> 2 -> 0


#### false negative
- Die Implementierung erkennt keine verschachtelten Threads und deren Abhängigkeit zueinander.\
  So wird bei einem Aufruf mit Thread t0 und seinem Subthread, die die Mutexe in umgekehrter Lockreihenfolge wie ein anderer Thread t1 locken, keine Warnmeldung ausgegeben.\
  Obwohl hier ein Zyklus und dadurch ein Deadlock entstehen kann.

  | Aufruf | Lock | Unlock |
  |:-------------------:|:------------:|:----------------:|
  Aufruf mit Thread 1: | 0 und starte Thread subThread | 0
  Aufruf mit Thread subThread: | 1 | 1
  Aufruf mit Thread 2: | 1 -> 0 | 0 -> 1

#### Deadlock
- Die Implementierung erkennt keinen Deadlock, wenn das Programm tatsächlich, während der Ausführung, in ein Deadlock endet.\
  So bleibt bei einem Aufruf mit Thread t0, das Mutex 0 lockt und dann ein Subthread startet, welches Mutex 1 und 0 locken möchte, in einem Deadlock stecken.

  | Aufruf | Lock | Unlock |
  |:-------------------:|:------------:|:----------------:|
  Aufrufender Thread: | 0 | 0
  Aufruf mit subThread: | 0 -> 1 | 1 -> 0
  

### Anwendung der Implementierung
Anhand des Testbeispiels test_FourThreads_MoreCycles() wird die Anwendung erklärt.\
Im Beispiel werden insgesamt 4 Threads mit unterschiedlicher Lockreihenfolge mit 3 Mutexen aufgerufen.\
Am Ende der Ausführung werden immer INFO, Lock graph und HISTORY ausgegeben.
Diese drei Ausgaben geben Informationen über die Ausführung der Threads, deren gelockten Mutexe, sowie ob es mindestens einen Zyklus gibt.\

- In der INFO steht, welche Threads noch welche Mutexe gelockt haben.\
  Da im Beispiel alle Threads ihre Mutexe am Ende releasen, hält kein Thread nach der Ausführung noch ein Mutex.
```
  *** INFO ***
  Thread 0 holds the following locks:
  Thread 1 holds the following locks:
  Thread 2 holds the following locks:
  Thread 3 holds the following locks:
```
- Der Lock graph gibt Auskunft, welche Mutexe verschachtelt gelockt werden.\
  Wenn es eine Kante von Mutex x zu Mutex y und eine Kante in die andere Richtung gibt, dann gibt es ein Zyklus und eine Fehlermeldung wird ausgegeben.\
  In diesem Beispiel gibt es zwei Zyklen und die Fehlermeldung wird ausgegeben.\
  Der erste Zyklus ist zwischen Mutex 0 und Mutex 1.\
  Der zweite Zyklus ist zwischen Mutex 0 und Mutex 2.
```
  Lock graph:
  0 --> 1
  0 --> 2
  1 --> 0
  2 --> 0
  *** cycle => potential deadlock !!! ***
```
- Mit der HISTORY kann nachvollzogen werden, welche Operation, welcher Thread mit welchem Mutex, in einer bestimmten Reihenfolge, gemacht hat.\
  Beispielhaft werden nun die ersten 4 Zeilen erklärt. Das gilt jedoch für jede weitere Zeile auch.

  Beim Aufruf 1 acquired der Thread T0 das Mutex M0\
  Beim Aufruf 2 acquired der Thread T0 das Mutex M1\
  Beim Aufruf 3 released der Thread T0 das Mutex M1\
  Beim Aufruf 4 released der Thread T0 das Mutex M0
```
 *** HISTORY ***
Call      Operation      T0 holds       T1 holds       T2 holds       T3 holds
1         T0 acq M0      0
2         T0 acq M1      0 1
3         T0 rel M1      0
4         T0 rel M0
5         T1 acq M1                     1
6         T1 acq M0                     1 0
7         T1 rel M0                     1
8         T1 rel M1
9         T2 acq M2                                    2
10        T2 acq M0                                    2 0
11        T2 rel M0                                    2
12        T2 rel M2
13        T3 acq M0                                                   0
14        T3 acq M2                                                   0 2
15        T3 rel M2                                                   0
16        T3 rel M0

Process finished with exit code 0
 ```
In dieser Ausgabe sind die Zyklen markiert. Das ist jedoch kein Teil der Implementierung, sondern dient nur zur Veranschaulichung.\

![](Zyklenaufgemalt.png)


### Zusammenfassung der gewonnenen Erkenntnisse
- besser Threads verstanden, wie sie funktionieren
- Parallelität (hyperthread) muss man viel beachten






-Interessante Files:
- [tsan_ilist.h](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/tsan/rtl/tsan_ilist.h)
- [tsan_mutexset.cpp](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/tsan/rtl/tsan_mutexset.cpp)
- [tsan_mutexset.h](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/tsan/rtl/tsan_mutexset.h)
- [sanitizer_Deadlock_detector_interface.h](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/sanitizer_common/sanitizer_deadlock_detector_interface.h)
- [sanitizer_Deadlock_detector1.cpp](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/sanitizer_common/sanitizer_deadlock_detector1.cpp)
- [sanitizer_Deadlock_detector2.cpp](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/sanitizer_common/sanitizer_deadlock_detector2.cpp)