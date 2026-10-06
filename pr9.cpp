#include <iostream>
#include <stdexcept>

const int SIZE = 6;

double originalData[SIZE];
double data[SIZE];

double headerSize = 20;
double maxPacketSize = 5000;
double minPacketSize = 64;

void printData(double data[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << data[i] << " байт";

        if (i < size - 1)
        {
            std::cout << ", ";
        }
    }

    std::cout << std::endl;
}

//ввод
void inputData(double data[], double originalData[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << "Введите размер пакета " << i + 1 << ": ";
        std::cin >> data[i];

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore();

            throw std::invalid_argument(
                "Некорректный размер пакета"
            );
        }

        if (data[i] <= 0)
        {
            throw std::invalid_argument(
                "Размер пакета должен быть больше 0"
            );
        }

        originalData[i] = data[i];
    }
}

//добавить размер заголовка
void operation1(double data[], int size)
{
    for (int i = 0; i < size; i++)
    {
        data[i] = data[i] + headerSize;
    }
}

//ограничить макс размер
void operation2(double data[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (data[i] > maxPacketSize)
        {
            data[i] = maxPacketSize;
        }
    }
}

//слишком большие заменить на 0
void operation3(double data[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (data[i] > maxPacketSize)
        {
            data[i] = 0;
        }
    }
}

//слишком маленькие заменить минимальным размером
void operation4(double data[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (data[i] < minPacketSize)
        {
            data[i] = minPacketSize;
        }
    }
}

//обработка
void applyProcessing(double data[], int size, void (*processor)(double[], int))
{
    if (processor != nullptr)
    {
        processor(data, size);
    } else
    {
        throw std::invalid_argument(
            "Обработчик не выбран"
        );
    }
}

//восстановление
void restoreData(
    double data[],
    double originalData[],
    int size )
{
    for (int i = 0; i < size; i++)
    {
        data[i] = originalData[i];
    }
}

int main()
{
    void (*processors[])(double[], int) = {
        operation1,
        operation2,
        operation3,
        operation4
    };

    int choice = -1;
    bool dataEntered = false;

    try
    {
        while (choice != 5)
        {
            std::cout << "\n1. Ввести данные" << std::endl;
            std::cout << "2. Показать данные" << std::endl;
            std::cout << "3. Выбрать и выполнить обработку" << std::endl;
            std::cout << "4. Восстановить исходные данные" << std::endl;
            std::cout << "5. Выход" << std::endl;
            std::cout << "Выбор: ";

            std::cin >> choice;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore();

                std::cout << "Некорректный ввод" << std::endl;
                continue;
            }

            if (choice == 1)
            {
                inputData(data, originalData, SIZE);
                dataEntered = true;

                std::cout << "Данные введены" << std::endl;
            }
            else if (choice == 2)
            {
                if (!dataEntered)
                {
                    std::cout << "Сначала введите данные" << std::endl;
                } else
                {
                    std::cout << "\nИсходный массив: ";
                    printData(originalData, SIZE);

                    std::cout << "Текущий массив: ";
                    printData(data, SIZE);
                }
            }
            else if (choice == 3)
            {
                if (!dataEntered)
                {
                    std::cout << "Сначала введите данные" << std::endl;
                } else
                {
                    int operation = 0;

                    std::cout << "\n1. Добавить размер заголовка" << std::endl;
                    std::cout << "2. Ограничить максимальный размер пакета" << std::endl;
                    std::cout << "3. Большие пакеты заменить на 0" << std::endl;
                    std::cout << "4. Маленькие пакеты заменить минимальным размером" << std::endl;
                    std::cout << "Выберите операцию: ";

                    std::cin >> operation;

                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cin.ignore();

                        std::cout << "Некорректный ввод" << std::endl;
                    }
                    else if (operation >= 1 && operation <= 4)
                    { 
                        void (*processor)(double[], int) = nullptr;

                        processor = processors[operation - 1];

                        std::cout << "\nМассив до обработки: ";
                        printData(data, SIZE);

                        applyProcessing(data, SIZE, processor);

                        std::cout << "Массив после обработки: ";
                        printData(data, SIZE);
                    }
                    else
                    {
                        std::cout << "Неверный номер операции" << std::endl;
                    }
                }
            }
            else if (choice == 4)
            {
                if (!dataEntered)
                {
                    std::cout << "Сначала введите данные" << std::endl;
                } else
                {
                    restoreData(data, originalData, SIZE);

                    std::cout << "Исходные данные восстановлены" << std::endl;
                }
            }
            else if (choice == 5)
            {
                std::cout << "Выход" << std::endl;
            }
            else
            {
                std::cout << "Неверный пункт" << std::endl;
            }
        }
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}