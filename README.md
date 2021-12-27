# SoftwareDesignLab8

Необходимо реализовать интерфейс EventsStatistic, который считает события, происходящие в
системе. Реализация должна хранить статистику ровно за последний час и подсчитывать,
сколько событий каждого типа произошло в минуту.

## Сборка
### Менеджер пакетов Conan
#### При наличии python
```
pip install conan
```
#### Установка с сайта:


[conan](https://conan.io/downloads.html)

### Linux CLI

```
cmake -DCMAKE_BUILD_TYPE=Release -S. -Bbuild
cmake --build build
```

#### Тесты

```
build/Tests/Tests
```

