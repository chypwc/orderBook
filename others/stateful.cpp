#include <iostream>

/*
 Static functions in classes do not have access to class data members.
 Non-static functions have access to class data members.
 Static functions can be called without instantiating the class into an object.
 Non-static functions require an object, and you call them on that object.
*/

class Car {
public:
    Car();
    void accelerate();
    // static function can't access to state (member variables) -> can't remember anything
    // we send data (objects) to static function
    // it processes the data that sent into it, but remember nothing 
    // stand-alone function
    static int whichIsFaster(Car carA, Car carB);
private:
    float speed; // state
};

Car::Car() : speed(0.0f) {}

void Car::accelerate() {
    speed += 10.0;
    std::cout << "Current speed: " << speed << std::endl;
}

int Car::whichIsFaster(Car carA, Car carB) { // can't access "speed" state like accelerate() non-static member function
    return (carA.speed > carB.speed) ? 1 : 2;
}

int main() {
    Car car1{};
    car1.accelerate();
    Car car2{};
    car2.accelerate();
    car2.accelerate();

    std::cout << Car::whichIsFaster(car1, car2) << std::endl;
    return 0;
}
