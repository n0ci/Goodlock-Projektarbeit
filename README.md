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
// TODO

-Interessante Files:
- [tsan_ilist.h](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/tsan/rtl/tsan_ilist.h)
- [tsan_mutexset.cpp](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/tsan/rtl/tsan_mutexset.cpp)
- [tsan_mutexset.h](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/tsan/rtl/tsan_mutexset.h)
- [sanitizer_Deadlock_detector_interface.h](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/sanitizer_common/sanitizer_deadlock_detector_interface.h)
- [sanitizer_Deadlock_detector1.cpp](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/sanitizer_common/sanitizer_deadlock_detector1.cpp)
- [sanitizer_Deadlock_detector2.cpp](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/sanitizer_common/sanitizer_deadlock_detector2.cpp)