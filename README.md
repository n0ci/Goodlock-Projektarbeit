# Goodlock-Projektarbeit
Im Rahmen dieser Projektarbeit arbeiten Marie Fiederlein und Fabio Nocera gemeinsam am Thema GoodLock. Betreut wird das Thema von Herrn Prof. Dr. Martin Sulzmann.

GoodLock ist ein Algorithmus, welcher dazu dient Programmverhalten dynamisch zu analysieren und Deadlocks zu erkennen. Das Ziel ist es sich mit dem Algorithmus, der Funktionsweise, dem Einsatz und einer eigenen Implementierung zu beschäftigen.

Hierbei ist der schriftliche Teil in drei grobe Teile untergliedert:
1. Hintergrundmaterial und Recherchen
   - [x] Readme mit Grundstruktur anlegen
   - [ ] Materialbeschaffung abschließen
2. Thread Sanitizer (Tsan) und dessen Implementierung
   - [ ] TSan verstehen und beschreiben
   - [ ] Eigene Beispiele mit Tsan durchführen und beschreiben
   - [ ] False Negative Fall für TSan überprüfen
3. Eigenständige Implementierung in C++
   - [x] Gedanken zur Struktur machen
   - [ ] Struktur schriftlich Festhalten
   - [ ] Implementieren
   
## Hintergrundmaterial und Recherchen
### Wissenschaftliche Arbeiten/Paper

1. Der Algorithmus für die Detektion von Deadlocks und data Races mit maximal 2 Threads wird erklärt und in Java implementiert.
   - [Using Runtime Analysis to Guide Model Checking of Java Programs](https://ti.arc.nasa.gov/m/pub-archive/177h/0177%20(Havelund).pdf)
2. Der Algorithmus für die Detektion von Deadlocks mit n Threads wird erklärt und in Java implementiert.
   - [Detecting Potential Deadlocks with Static Analysis and Run-Time Monitoring](https://www.research.ibm.com/haifa/Workshops/PADTAD2005/papers/article.pdf)
3. Der Algorithmus des ThreadSanitizer wird erklärt und seine Einsatzgebiete vom Testen von multithreaded C++ Code aufgezeigt.
   - [ThreadSanitizer – data race detection in practice](https://static.googleusercontent.com/media/research.google.com/de//pubs/archive/35604.pdf)
4. Es werden zwei Analyse Tools, ThreadSanitizer und ThreadSafety, die zum Testen von multithreaded Programmen genutzt werden, näher betrachtet und deren Vor- und Nachteile aufgezeigt.
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
  - [Zwei Threads in Abhängigkeit](https://sulzmann.github.io/AutonomeSysteme/lec-deadlock.html#(3))

### Implementierungen
1. Implementierung verschiedener Sanitizer unter anderem den ThreadSanitizer in den Sprachen C++ und Go. Die zugrundeliegende Implementierung vom ThreadSanitizer ist aus dem LLVM-Project.
   - [google/sanitizers](https://github.com/google/sanitizers)
2. Die grundlegende Implementierung von ThreadSanitizer in C++. Die LLVM Compiler Infrastructure ist eine Sammlung von modularen und wiederverwendbaren Compilern und toolchain-Techniken.
   - [Docs](https://clang.llvm.org/docs/ThreadSanitizer.html)
   - [Codebase](https://github.com/llvm/llvm-project/tree/main/compiler-rt/lib/tsan)
   
### Random Link Sammlung:
- [Tests in Tsan (LLVM)](https://github.com/llvm/llvm-project/tree/main/compiler-rt/test/tsan)
- [Stackoverflow Probleme mit TSan](https://stackoverflow.com/questions/tagged/thread-sanitizer?tab=newest&page=1&pagesize=15)

## ThreadSanitizer und dessen Implementierung (Clang/LLVM)
Interessante Files:
- [tsan_ilist.h](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/tsan/rtl/tsan_ilist.h)
- [tsan_mutexset.cpp](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/tsan/rtl/tsan_mutexset.cpp)
- [tsan_mutexset.h](https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/tsan/rtl/tsan_mutexset.h)
## Eigenständige Implementierung in C++