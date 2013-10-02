#include <string>

#include "sticker.h"

using std::string;

Sticker::Sticker(const string& name) {
    this->name = name;
    this->logo = "";
    this->article = "";
    this->type = "";
    this->number = 0;
    this->voltage = "";
    this->IP = "";
    this->flame = 0;
    this->led_type = 0;
    this->protection = 0;
}

Sticker::Sticker(const Sticker& S) {
    this->name = S.name;
    this->logo = S.logo;
    this->article = S.article;
    this->type = S.type;
    this->number = S.number;
    this->voltage = S.voltage;
    this->IP = S.IP;
    this->flame = S.flame;
    this->led_type = S.led_type;
    this->protection = S.protection;
}

Sticker::~Sticker() { }

Sticker& Sticker::operator=(const Sticker& rhs) {
    if (this != &rhs) {
        this->name = rhs.name;
        this->logo = rhs.logo;
        this->article = rhs.article;
        this->type = rhs.type;
        this->number = rhs.number;
        this->voltage = rhs.voltage;
        this->IP = rhs.IP;
        this->flame = rhs.flame;
        this->led_type = rhs.led_type;
        this->protection = rhs.protection;            
    }
    return *this;
}

bool Sticker::operator==(const Sticker& rhs) const {
    return ((this->name       == rhs.name)       &&
            (this->logo       == rhs.logo)       &&
            (this->article    == rhs.article)    &&
            (this->type       == rhs.type)       &&
            (this->number     == rhs.number)     &&
            (this->voltage    == rhs.voltage)    &&
            (this->IP         == rhs.IP)         &&
            (this->flame      == rhs.flame)      &&
            (this->led_type   == rhs.led_type)   &&
            (this->protection == rhs.protection)); 
}

bool Sticker::operator!=(const Sticker& rhs) const {
    return !(*this == rhs);
}

void Sticker::setName(const string& name) {
    this->name = name;
}

const string& Sticker::getName() const {
    return this->name;
}

void Sticker::setLogo(const string& logo) {
    this->logo = logo;
}

const string& Sticker::getLogo() const {
    return this->logo;
}

void Sticker::setArticle(const string& article) {
    this->article = article;
}

const string& Sticker::getArticle() const {
    return this->article;
}

void Sticker::setType(const string& type) {
    this->type = type;
}

const string& Sticker::getType() const {
    return this->type;
}

void Sticker::setNumber(int number) {
    this->number = number;
}

int Sticker::getNumber() const {
    return this->number;
}

void Sticker::setVoltage(const string& voltage) {
    this->voltage = voltage;
}

const string& Sticker::getVoltage() const {
    return this->voltage;
}

void Sticker::setIP(const string& IP) {
    this->IP = IP;
}

const string& Sticker::getIP() const {
    return this->IP;
}

void Sticker::setFlame(int flame) {
    this->flame = flame;
}

int Sticker::getFlame() const {
    return this->flame;
}

void Sticker::setLedType(int led_type) {
    this->led_type = led_type;
}

int Sticker::getLedType() const {
    return this->led_type;
}

void Sticker::setProtection(int protection) {
    this->protection = protection;
}

int Sticker::getProtection() const {
    return this->protection;
}

string Sticker::toString() const {
    string article = this->article;
    for (size_t i = 0; i < article.size(); ++i) {
        if (article[i] == '/') {
            article[i] = '_';
        }
    }
    return this->name + " " + article;    
}
