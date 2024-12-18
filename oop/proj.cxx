#include <iostream>
#include <string>

using namespace std;

//наблюдатель
class Observer{
public:
    virtual void update(const string& message) = 0;
    virtual ~Observer() = default;};

//Subject (класс, который уведомляет наблюдателя)
class Subject{
public:
    virtual void addObserver(Observer* observer) = 0;
    virtual void removeObserver() = 0;
    virtual void notifyObserver(const string& message) = 0;
    virtual ~Subject() = default;};

//класс номеров
class Room{
public:
    virtual string getType() const = 0; // возвращает тип номера
    virtual ~Room() = default;};          // деструктор

//конкретные классы номеров
class LuxuryRoom : public Room{
public:
    string getType() const override{
        return "Люкс";}};

class EconomyRoom : public Room{
public:
    string getType() const override{
        return "Эконом";}};

//фабрика
class RoomFactory{
public:
    static Room* createRoom(const string& roomType){
        if(roomType == "Люкс"){
            return new LuxuryRoom();} 
        else if (roomType == "Эконом"){
            return new EconomyRoom();} 
        else{
            throw invalid_argument("Введите 'Люкс' или 'Эконом'");}}};

//класс брони
class Booking : public Subject{
private:
    string fullName;
    string phoneNumber;
    string checkInDate;
    string checkOutDate;
    Room* room; // указатель на объект класса Room
    int numberOfPeople;
    
    Observer* observer = nullptr; //наблюдатель

public:
    //конструктор
    Booking(const string& name, const string& phone, const string& checkIn, const string& checkOut, Room* room, int people)
        : fullName(name), phoneNumber(phone), checkInDate(checkIn), checkOutDate(checkOut), room(room), numberOfPeople(people) {}

    //метод добавление наблюдателя
    void addObserver(Observer* newObserver) override{
        observer = newObserver;}

    //метод удаление наблюдателя
    void removeObserver() override{
        observer = nullptr;}

    //уведомление наблюдателя
    void notifyObserver(const string& message) override{
        if(observer){
            observer->update(message);}}

    //метод подтверждения брони
    void confirmBooking() {
        cout << "\nНомер забронирован на имя: " << fullName << endl;
        cout << "Тип номера: " << room->getType() << endl;
        cout << "Количество людей: " << numberOfPeople << endl;
        cout << "Дата заезда: " << checkInDate << endl;
        cout << "Дата выезда: " << checkOutDate << endl;

        //уведомляем наблюдателя
        notifyObserver("\nБронирование подтверждено для: " + fullName);}

    //деструктор
    ~Booking(){
        delete room;}};

//поведение наблюдателя
class BookingObserver : public Observer{
private:
    string observerName;

public:
    BookingObserver(const string& name) : observerName(name) {}

    //уведомления
    void update(const string& message) override {
        cout << observerName << " получил уведомление: " << message << endl;}};

int main(){
    string fullName, phoneNumber, checkInDate, checkOutDate, roomType;
    int numberOfPeople;

    cout << "Введите ФИО: ";
    getline(cin, fullName);

    cout << "Введите номер телефона: ";
    getline(cin, phoneNumber);

    cout << "Введите дату заезда(дд/мм/гггг): ";
    getline(cin, checkInDate);

    cout << "Введите дату выезда(дд/мм/гггг): ";
    getline(cin, checkOutDate);

    cout << "Введите тип номера(Люкс/Эконом): ";
    getline(cin, roomType);

    cout << "Введите количество человек: ";
    cin >> numberOfPeople;

    //объект комнаты через фабрику
    Room* room = RoomFactory::createRoom(roomType);

    //объект брони
    Booking booking(fullName, phoneNumber, checkInDate, checkOutDate, room, numberOfPeople);

    //создаем наблюдателя
    BookingObserver observer1("Наблюдатель");

    //добавляем наблюдателя
    booking.addObserver(&observer1);

    //подтверждение брони
    booking.confirmBooking();

    return 0;}
