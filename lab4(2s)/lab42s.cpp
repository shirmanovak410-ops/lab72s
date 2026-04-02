//
// Created by Ксения on 29.03.2026.
//
#include <iostream>
#include <string>

class Car {
protected:
    std::string model;
    int maxSpeed;

public:
    Car(const std::string& modelName, int speed)
        : model(modelName), maxSpeed(speed) {}

    virtual void print() const {
        std::cout << "Car: " << model
                  << ", V: " << maxSpeed ;
    }

    void printStatic() const {
        std::cout << "Car: " << model
                  << ", V: " << maxSpeed;
    }
    virtual ~Car() {}
};

class Bus : public Car {
private:
    int passengerCapacity;

public:
    Bus(const std::string& modelName, int speed, int capacity)
        : Car(modelName, speed), passengerCapacity(capacity) {}

    void print() const override {
        Car::print();
        std::cout << ", passengers: " << passengerCapacity;
    }

    void printStatic() const {
        Car::printStatic();
        std::cout << ", passengers: " << passengerCapacity;
    }
};

int main() {
    Car car("Skoda", 280);
    Bus bus("Volvo", 120, 45);
    Car* ptr;

    ptr = &car;
    ptr->printStatic();
    std::cout << std::endl;

    ptr = &bus;
    ptr->printStatic();
    std::cout << std::endl;

    ptr = &car;
    ptr->print();
    std::cout << std::endl;

    ptr = &bus;
    ptr->print();
    std::cout << std::endl;

    return 0;
}