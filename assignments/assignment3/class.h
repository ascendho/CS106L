#ifndef CLASS_H
#define CLASS_H

#include <string>

class Book
{
public:
    // Default constructor
    Book();

    // Custom constructor
    Book(const std::string &title, const std::string &author, int pages);

    // Getters (const-correct)
    std::string getTitle() const;
    std::string getAuthor() const;
    int getPages() const;

    // Setters
    void setTitle(const std::string &title);
    void setAuthor(const std::string &author);
    void setPages(int pages);

private:
    std::string title_;
    std::string author_;
    int pages_;

    // Private helper: normalize/trim title
    void normalizeTitle();

    // Private validator for pages
    bool validatePages(int pages) const;
};

#endif // CLASS_H
