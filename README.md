## 📘 Лабораторна робота

**Тема:** Практичне застосування системи контролю версій Git та юніт-тестування  
**Звіт:** [Звіт_ЛР_Unit_Tests.docx](./Звіт_ЛР_Unit_Tests.docx)

У звіті описано:
- створення репозиторію та гілок;
- розробку юніт-тестів (GoogleTest);
- результати тестування та створення Pull Request.

Структура проєкту
Unit-tests/
├── tests/                # Тести (GoogleTest)
│   └── test_smoke.cpp
├── CMakeLists.txt        # Збірка проєкту
├── README.md             # Опис проєкту
└── Звіт_ЛР_Unit_Tests.docx  # Звіт лабораторної роботи


Як запустити тести
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure

Очікуваний результат:

[==========] Running 1 test from 1 test case.
[ RUN      ] SmokeTest.BasicArithmetic
[       OK ] SmokeTest.BasicArithmetic (0 ms)
[  PASSED  ] 1 test.
