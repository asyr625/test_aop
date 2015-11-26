#ifndef NON_COPYABLE_H
#define NON_COPYABLE_H

class Non_Copyable
{
protected:
    Non_Copyable() = default;
    ~Non_Copyable() = default;

    Non_Copyable(const Non_Copyable&) = delete;
    Non_Copyable& operator=(const Non_Copyable&) = delete;
};

#endif
