#ifndef STICKER_H
#define STICKER_H

#include <string>

using std::string;

class Sticker {
public:
    Sticker(const string& name);
    Sticker(const Sticker& S);

    ~Sticker();

    Sticker& operator=(const Sticker& rhs);

    bool operator==(const Sticker& rhs) const;
    bool operator!=(const Sticker& rhs) const;

    void setName(const string& name);
    const string& getName() const;
    void setLogo(const string& logo);
    const string& getLogo() const;
    void setArticle(const string& article);
    const string& getArticle() const;
    void setType(const string& type);
    const string& getType() const;
    void setNumber(int number);
    int getNumber() const;
    void setVoltage(const string& voltage);
    const string& getVoltage() const;
    void setIP(const string& IP);
    const string& getIP() const;
    void setFlame(int flame);
    int getFlame() const;
    void setLedType(int led_type);
    int getLedType() const;
    void setProtection(int protection);
    int getProtection() const;

protected:
    string name;
    string logo;
    string article;
    string type;
    int number;
    string voltage;
    string IP;
    int flame;
    int led_type;
    int protection;
};

#endif