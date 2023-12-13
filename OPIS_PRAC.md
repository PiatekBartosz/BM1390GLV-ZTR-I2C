# Zakres w kolejności chronologicznej
- utworzenie projektu: stworzenie repozytorium, projekt w CMAKE, wstępna koncepcja działania aplikacji: ```./soultion_ideas/initial_solution_idea.png```
- praca nad aplikację czytającą plik z danymi linijka po linijce, struct symulujący rejestr
- data parsing - wyłuskanie danych o temperaturze i ciśnieniu ze stringa
- rozbudowa projektu CMAKE - dodanie drugiej aplikacji wykonywalnej (MainApp + SensorSimulator)
- API/interfejs dla I2C oraz drivera BM1390GLV-ZTR 
- implementacja API/interfejsu dla I2C i drivera
- praca nad socketami (w celu wykorzystania oprogramowania Hercules, dostosowano CMAKE do kompilacji zarówno na Linuxie, jak i Windowsie)
- praca nad algorytmem przejścia o jedno piętro i górę i w dół

# W jakie sposób rozwijałem projekt
- CMAKE
- GCC
- clang-formater
- VS Code + CMAKE Extension
- Hercules SETUP (Windows only)