#ifndef STICKER_H
#define STICKER_H

#include <string>

class Sticker {
public:
    Sticker(const std::string& name);
    Sticker(const Sticker& S);

    ~Sticker();

    Sticker& operator=(const Sticker& rhs);

    bool operator==(const Sticker& rhs) const;
    bool operator!=(const Sticker& rhs) const;

    void setName(const std::string& name);
    const std::string& getName() const;
    void setLogo(const std::string& logo);
    const std::string& getLogo() const;
    void setArticle(const std::string& article);
    const std::string& getArticle() const;
    void setType(const std::string& type);
    const std::string& getType() const;
    void setNumber(int number);
    int getNumber() const;
    void setVoltage(const std::string& voltage);
    const std::string& getVoltage() const;
    void setIP(const std::string& IP);
    const std::string& getIP() const;
    void setFlame(int flame);
    int getFlame() const;
    void setLedType(int led_type);
    int getLedType() const;
    void setProtection(int protection);
    int getProtection() const;

    std::string toString() const;

protected:
    std::string name;
    std::string logo;
    std::string article;
    std::string type;
    int number;
    std::string voltage;
    std::string IP;
    int flame;
    int led_type;
    int protection;
};



#endif