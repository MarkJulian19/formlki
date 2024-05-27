# Построение ДКА по регулярному выражению

Необходимо скачать [предоставленное окружение](https://earth.ispras.ru/public-archives/automatoncourse-2022/re2dfa.zip) и реализовать функцию `re2dfa` в файле `task.cpp`, принимающую в качестве аргумента корректное регулярное выражение `s` в виде `std::string` и возвращающую детерминированный конечный автомат в виде DFA, задающий тот же язык, что и регулярное выражение `s`.

*Окружение* содержит реализации алфавита в виде класса `Alphabet` (этот класс позволяет автоматически получать алфавит, над которым задано регулярное выражение) и ДКА в виде класса DFA (этот класс помогает вывести автомат в правильном формате). API этих классов приведен в файле `api.hpp`. Состояния в DFA идентифицируются непустыми строковыми именами, присваиваемыми им при создании. 

Тестовые регулярные выражения всегда корректны и состоят из символов алфавита (в алфавит могут входить буквы латинского алфавита и цифры), скобок, `|` - операция объединения, `*` - итерация. Специальный символ пустой строки (эпсилон) не используется: выражение `(ε|a)b*` записывается как `(|a)b*`. Пустая строка считается регулярным выражением, задающим пустое слово, а не пустым регулярным выражением. Можно считать, что тестовые регулярные выражения содержат не более 1000 символов. 

Функция main в файле main.cpp считывает регулярное выражение из файла `re2dfa.in`, передает его в функцию `re2dfa`, после чего выводит построенный ДКА в текстовом виде в файл `re2dfa.out` в следующем формате: на первой строке перечисляются все символы алфавита, затем на отдельных строчках перечисляются все состояния в формате "[имя_состояния]" или же "[[имя_состояния]]" для финальных состояний. Первое упомянутое состояние считается начальным. Затем перечисляются все переходы в автомате в формате "[имя_начального_состояния] символ_перехода [имя_конечного_состояния]", по одному переходу на строчку. 

Сборка окружения выполняется с помощью CMake или bash-скрипта `quick-setup.sh`. Получаемый бинарный исполняемый файл - `re2dfa`.