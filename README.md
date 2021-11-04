# Goodlock-Projektarbeit

Im Rahmen dieser Projektarbeit arbeiten Marie Fiederlein und Fabio Nocera gemeinsam am Thema GoodLock. Betreut wird das Thema von Herrn Prof. Dr. Martin Sulzmann.

GoodLock ist ein Algorithmus, welcher dazu dient Programmverhalten dynamisch zu analysieren und Deadlocks zu erkennen. Das Ziel ist es sich mit dem Algorithmus, der Funktionsweise, dem Einsatz und einer eigenen Implementierung zu beschäftigen.

Hierbei ist der schriftliche Teil in drei grobe Teile untergliedert:
1. Hintergrundmaterial und Recherchen

- [x] Readme mit Grundstruktur anlegen
- [x] Materialbeschaffung abschließen

2. Thread Sanitizer (Tsan) und dessen Implementierung

- [ ] Eigene Beispiele mit Tsan durchführen und beschreiben
- [ ] False Negative und false Positives Fälle für TSan überprüfen
- [ ] TSan verstehen und beschreiben

3. Eigenständige Implementierung in C++

- [x] Gedanken zur Struktur machen
- [ ] Struktur schriftlich Festhalten
- [ ] Implementieren

## Hintergrundmaterial und Recherchen

### Wissenschaftliche Arbeiten/Paper

- Der Algorithmus für die Detektion von Deadlocks und data Races mit maximal 2 Threads wird erklärt und in Java implementiert.
   - [Using Runtime Analysis to Guide Model Checking of Java Programs](<https://ti.arc.nasa.gov/m/pub-archive/177h/0177%20(Havelund).pdf>)
- Der Algorithmus für die Detektion von Deadlocks mit n Threads wird erklärt und in Java implementiert.
   - [Detecting Potential Deadlocks with Static Analysis and Run-Time Monitoring](https://www.research.ibm.com/haifa/Workshops/PADTAD2005/papers/article.pdf)
- Der Algorithmus des ThreadSanitizer wird erklärt und seine Einsatzgebiete vom Testen von multithreaded C++ Code aufgezeigt.
   - [ThreadSanitizer – data race detection in practice](https://static.googleusercontent.com/media/research.google.com/de//pubs/archive/35604.pdf)
- Es werden zwei Analyse Tools, ThreadSanitizer und ThreadSafety, die zum Testen von multithreaded Programmen genutzt werden, näher betrachtet und deren Vor- und Nachteile aufgezeigt.
   - [How Developers Use Data Race Detection Tools](https://static.googleusercontent.com/media/research.google.com/en//pubs/archive/43217.pdf)

### Beispiele

- Eine Beschreibung einiger der häufigsten Daten Races, die vom ThreadSanitizer erkannt werden.
   - [ThreadSanitizerPopularDataRaces](https://github.com/google/sanitizers/wiki/ThreadSanitizerPopularDataRaces)
- False Positives:
   - [ThreadSanitizer gives false positive warning about deadlock](https://gist.github.com/spetrunia/77274cf2d5848e0a7e090d622695ed4e)
   - [Thread Sanitizer false positive on lock-order-inversion (potential deadlock)](https://github.com/google/sanitizers/issues/1419)
   - [False lock-order-inversion reports for locks taken in a single thread](https://github.com/google/sanitizers/issues/488)
   - [tsan: False positive for lock-order-inversion for lock cycles with asymmetrical unlocking](https://github.com/google/sanitizers/issues/814)
- False Negatives:
   - [Zwei Threads in Abhängigkeit](<https://sulzmann.github.io/AutonomeSysteme/lec-deadlock.html#(3)>)
   - [Weitere häufige false Negatives werden beschrieben](https://groups.google.com/g/thread-sanitizer/c/mB73m6Nltaw)

### Implementierungen

- Implementierung verschiedener Sanitizer unter anderem den ThreadSanitizer in den Sprachen C++ und Go. Die zugrundeliegende Implementierung vom ThreadSanitizer ist aus dem LLVM-Project. Es wird sehr grob beschrieben, wie Tsan funktioniert.
   - [google/sanitizers](https://github.com/google/sanitizers)
   - [google/sanitizers/wiki/ThreadSanitizerAlgorithm](https://github.com/google/sanitizers/wiki/ThreadSanitizerAlgorithm)
- Die grundlegende Implementierung von ThreadSanitizer in C++ von LLVM. Die LLVM Compiler Infrastructure ist eine Sammlung von modularen und wiederverwendbaren Compilern und toolchain-Techniken.
   - [Docs](https://clang.llvm.org/docs/ThreadSanitizer.html)
   - [Codebase](https://github.com/llvm/llvm-project/tree/main/compiler-rt/lib/tsan)
- Tests aus dem Repository von LLVM.
   - [llvm-project/compiler-rt/test/tsan](https://github.com/llvm/llvm-project/tree/main/compiler-rt/test/tsan)

### Diskussionen und Erklärungen

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

## ThreadSanitizer und dessen Implementierung (Clang/LLVM)

Interessante Files:

- [tsan_ilist.h](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/tsan/rtl/tsan_ilist.h)
- [tsan_mutexset.cpp](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/tsan/rtl/tsan_mutexset.cpp)
- [tsan_mutexset.h](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/tsan/rtl/tsan_mutexset.h)

## Eigenständige Implementierung in C++
