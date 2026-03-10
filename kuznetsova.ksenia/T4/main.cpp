#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <string>
#include <clocale>
#include <cmath>
#include "Rectangle.h"
#include "Ring.h"
#include "Square.h"
#include "CompositeShape.h"
#include"Shape.h"
#include"Point.h"

void printShapeInfo(const Shape& shape) {
    Point center = shape.getCenter();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << shape.getName() << ", ("
        << center.x << ", " << center.y << "), "
        << shape.getArea();
}

void printComposite(const CompositeShape& composite) {
    std::cout << "[";
    printShapeInfo(composite);
    std::cout << ";\n";

    for (size_t i = 0; i < composite.getSize(); ++i) {
        std::cout << "  ";
        printShapeInfo(*composite.getShape(i));
        if (i != composite.getSize() - 1) {
            std::cout << ",\n";
        }
        else {
            std::cout << "\n";
        }
    }
    std::cout << "]";
}

void testRectangle() {
    std::cout << "\nTЕСТИРОВАНИЕ ПРЯМОУГОЛЬНИКА\n";

    Rectangle rect(Point(1, 1), Point(5, 4));
    std::cout << "Создан прямоугольник с углами (1,1) и (5,4)\n";
    std::cout << "  ";
    printShapeInfo(rect);
    std::cout << "\n";

    std::cout << "Перемещение на (2, -1):\n";
    rect.move(2, -1);
    std::cout << "  ";
    printShapeInfo(rect);
    std::cout << "\n";

    std::cout << "Масштабирование x2:\n";
    rect.scale(2.0);
    std::cout << "  ";
    printShapeInfo(rect);
    std::cout << "\n";
}

void testRing() {
    std::cout << "\nТЕСТИРОВАНИЕ КОЛЬЦА\n";

    Ring ring(Point(3, 3), 5, 2);
    std::cout << "Создано кольцо с центром (3,3), внешний радиус=5, внутренний=2\n";
    std::cout << "  ";
    printShapeInfo(ring);
    std::cout << "\n";

    std::cout << "Перемещение на (-1, 2):\n";
    ring.move(-1, 2);
    std::cout << "  ";
    printShapeInfo(ring);
    std::cout << "\n";

    std::cout << "Масштабирование x0.5:\n";
    ring.scale(0.5);
    std::cout << "  ";
    printShapeInfo(ring);
    std::cout << "\n";
}

void testSquare() {
    std::cout << "\nТЕСТИРОВАНИЕ КВАДРАТА\n";

    Square square(Point(0, 0), 4);
    std::cout << "Создан квадрат с левым нижним углом (0,0) и стороной 4\n";
    std::cout << "  ";
    printShapeInfo(square);
    std::cout << "\n";

    std::cout << "Перемещение на (3, 2):\n";
    square.move(3, 2);
    std::cout << "  ";
    printShapeInfo(square);
    std::cout << "\n";

    std::cout << "Масштабирование x1.5:\n";
    square.scale(1.5);
    std::cout << "  ";
    printShapeInfo(square);
    std::cout << "\n";
}

void testCompositeOperations() {
    std::cout << "\nТЕСТИРОВАНИЕ ОПЕРАЦИЙ COMPOSITE\n";

    CompositeShape composite;

    composite.addShape(std::make_unique<Square>(Point(1, 1), 2));
    composite.addShape(std::make_unique<Ring>(Point(4, 4), 2, 1));
    composite.addShape(std::make_unique<Rectangle>(Point(2, 2), Point(5, 4)));

    std::cout << "Создана составная фигура из 3 элементов:\n";
    std::cout << "  Количество фигур: " << composite.getSize() << "\n";
    std::cout << "  Общая площадь: " << composite.getArea() << "\n";
    Point center = composite.getCenter();
    std::cout << "  Центр композита: (" << center.x << ", " << center.y << ")\n\n";

    std::cout << "Составляющие фигуры:\n";
    for (size_t i = 0; i < composite.getSize(); ++i) {
        Shape* shape = composite.getShape(i);
        Point c = shape->getCenter();
        std::cout << "  Фигура " << i + 1 << ": " << shape->getName()
            << ", центр (" << c.x << ", " << c.y << "), площадь="
            << shape->getArea() << "\n";
    }

    std::cout << "\nПеремещение композита на (1, -1):\n";
    composite.move(1, -1);

    std::cout << "Новые центры фигур:\n";
    for (size_t i = 0; i < composite.getSize(); ++i) {
        Shape* shape = composite.getShape(i);
        Point c = shape->getCenter();
        std::cout << "  Фигура " << i + 1 << ": (" << c.x << ", " << c.y << ")\n";
    }

    std::cout << "\nМасштабирование композита x2:\n";
    composite.scale(2.0);

    std::cout << "После масштабирования:\n";
    for (size_t i = 0; i < composite.getSize(); ++i) {
        Shape* shape = composite.getShape(i);
        Point c = shape->getCenter();
        std::cout << "  Фигура " << i + 1 << ": " << shape->getName()
            << ", центр (" << c.x << ", " << c.y << "), площадь="
            << shape->getArea() << "\n";
    }
}

void testCompositeScaleExample() {
    std::cout << "\nТЕСТИРОВАНИЕ МАСШТАБИРОВАНИЯ ИЗ ПРИМЕРА\n";
    std::cout << "Пример: Circle (2,2) и Rectangle (8,2)\n\n";

    CompositeShape composite;

    //круг (используем Ring с innerR=0)
    composite.addShape(std::make_unique<Ring>(Point(2, 2), 1, 0));

    //прямоугольник с центром в (8,2)
    composite.addShape(std::make_unique<Rectangle>(Point(7, 1), Point(9, 3)));

    std::cout << "ДО маштабирования (factor=2):\n";
    std::cout << "  Центр композита: ("
        << composite.getCenter().x << ", "
        << composite.getCenter().y << ")\n";

    for (size_t i = 0; i < composite.getSize(); ++i) {
        Shape* shape = composite.getShape(i);
        Point c = shape->getCenter();
        std::cout << "  Фигура " << i + 1 << ": " << shape->getName()
            << ", центр (" << c.x << ", " << c.y << ")\n";
    }

    std::cout << "\nВыполняем scale(2)...\n";
    composite.scale(2.0);

    std::cout << "\nПОСЛЕ маштабирования:\n";
    std::cout << "Центр композита: ("
        << composite.getCenter().x << ", "
        << composite.getCenter().y << ")\n";

    for (size_t i = 0; i < composite.getSize(); ++i) {
        Shape* shape = composite.getShape(i);
        Point c = shape->getCenter();
        std::cout << "Фигура " << i + 1 << ": " << shape->getName()
            << ", центр (" << c.x << ", " << c.y << "), площадь="
            << shape->getArea() << "\n";
    }

    std::cout << "\nОжидаемый результат из примера:\n";
    std::cout << "Круг должен переместиться в (-1,2)\n";
    std::cout << "Прямоугольник должен переместиться в (11,2)\n";
}

void testBoundingBox() {
    std::cout << "\nТЕСТИРОВАНИЕ BOUNDING BOX\n";

    CompositeShape composite;

    // Добавляем фигуры по углам
    composite.addShape(std::make_unique<Square>(Point(0, 0), 1));
    composite.addShape(std::make_unique<Square>(Point(8, 0), 1));
    composite.addShape(std::make_unique<Square>(Point(0, 6), 1));
    composite.addShape(std::make_unique<Square>(Point(8, 6), 1));

    std::cout << "Фигуры расположены по углам:\n";
    std::cout << "(0,0), (8,0), (0,6), (8,6)\n";

    Point center = composite.getCenter();
    std::cout << "Центр композита (по bounding box центров): ("
        << center.x << ", " << center.y << ")\n";
    std::cout << "Ожидаемый центр: (4, 3)\n";
}

void testMainTask() {
    std::cout << "\n";
    std::cout << "ОСНОВНОЕ ЗАДАНИЕ (ТРЕБУЕМЫЙ ВЫВОД)\n";

    std::vector<std::unique_ptr<Shape>> shapes;

    //создаем 5+ фигур
    shapes.push_back(std::make_unique<Rectangle>(Point(0, 0), Point(4, 3)));//прямоугольник 4x3
    shapes.push_back(std::make_unique<Ring>(Point(5, 5), 5, 2));//кольцо
    shapes.push_back(std::make_unique<Square>(Point(2, 2), 3));//квадрат 3x3
    shapes.push_back(std::make_unique<Rectangle>(Point(-3, -2), Point(1, 2)));//прямоугольник 4x4
    shapes.push_back(std::make_unique<Ring>(Point(-2, 4), 3, 1));//маленькое кольцо

    //создаем составную фигуру (минимум одна)
    auto composite = std::make_unique<CompositeShape>();
    composite->addShape(std::make_unique<Square>(Point(1, 1), 2));//квадрат 2x2
    composite->addShape(std::make_unique<Ring>(Point(3, 3), 2, 1));//кольцо
    composite->addShape(std::make_unique<Rectangle>(Point(2, 0), Point(4, 2)));//прямоугольник 2x2
    shapes.push_back(std::move(composite));

    std::cout << "ДО маштабирования (x1):\n";
    std::cout << "------------------------\n";
    for (const auto& shape : shapes) {
        if (shape->getName() == "COMPOSITE") {
            printComposite(static_cast<const CompositeShape&>(*shape));
        }
        else {
            std::cout << "  ";
            printShapeInfo(*shape);
        }
        std::cout << "\n";
    }

    std::cout << "\nМасштабирование x2...\n\n";

    //масштабируем все фигуры в 2 раза
    for (auto& shape : shapes) {
        shape->scale(2.0);
    }

    std::cout << "ПОСЛЕ маштабирования (x2):\n";
    std::cout << "---------------------------\n";
    for (const auto& shape : shapes) {
        if (shape->getName() == "COMPOSITE") {
            printComposite(static_cast<const CompositeShape&>(*shape));
        }
        else {
            std::cout << "  ";
            printShapeInfo(*shape);
        }
        std::cout << "\n";
    }
}

void testEdgeCases() {
    std::cout << "\nТЕСТИРОВАНИЕ ГРАНИЧНЫХ СЛУЧАЕВ\n";

    std::cout << "Пустой CompositeShape:\n";
    CompositeShape empty;
    std::cout << "Размер: " << empty.getSize() << "\n";
    std::cout << "Пустой? " << (empty.isEmpty() ? "да" : "нет") << "\n";
    std::cout << "Площадь: " << empty.getArea() << "\n";
    Point center = empty.getCenter();
    std::cout << "Центр: (" << center.x << ", " << center.y << ")\n";

    std::cout << "\nПопытка получить элемент по индексу:\n";
    try {
        empty.getShape(0);
        std::cout << "ОШИБКА: должно быть исключение\n";
    }
    catch (const std::out_of_range& e) {
        std::cout << "Исключение: " << e.what() << "\n";
    }

    std::cout << "\nComposite с одной фигурой:\n";
    CompositeShape single;
    single.addShape(std::make_unique<Square>(Point(2, 2), 2));
    std::cout << "Размер: " << single.getSize() << "\n";
    std::cout << "Площадь: " << single.getArea() << "\n";
    center = single.getCenter();
    std::cout << "Центр: (" << center.x << ", " << center.y << ")\n";

    Shape* shape = single.getShape(0);
    Point c = shape->getCenter();
    std::cout << "Центр фигуры: (" << c.x << ", " << c.y << ")\n";

    std::cout << "\nМасштабирование x3:\n";
    single.scale(3.0);
    std::cout << "Новая площадь: " << single.getArea() << "\n";
    center = single.getCenter();
    std::cout << "Новый центр: (" << center.x << ", " << center.y << ")\n";
    shape = single.getShape(0);
    c = shape->getCenter();
    std::cout << "Новый центр фигуры: (" << c.x << ", " << c.y << ")\n";
}

int main() {
    setlocale(LC_ALL, "RUS");

    std::cout << "\n";
    std::cout << "ТЕСТИРОВАНИЕ ПРОГРАММЫ 'ФИГУРЫ'\n";

    //тестирование отдельных фигур
    testRectangle();
    testRing();
    testSquare();

    //тестирование CompositeShape
    testCompositeOperations();
    testCompositeScaleExample();
    testBoundingBox();
    testEdgeCases();

    //основное задание
    testMainTask();
    return 0;
}