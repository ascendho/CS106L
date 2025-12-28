#include "class.h"
#include <algorithm>
#include <cctype>

Book::Book() : title_("Untitled"), author_("Unknown"), pages_(0) {}

Book::Book(const std::string &title, const std::string &author, int pages)
    : title_(title), author_(author), pages_(pages)
{
    normalizeTitle();
    if (!validatePages(pages_))
    {
        pages_ = 0;
    }
}

std::string Book::getTitle() const { return title_; }
std::string Book::getAuthor() const { return author_; }
int Book::getPages() const { return pages_; }

void Book::setTitle(const std::string &title)
{
    title_ = title;
    normalizeTitle();
}
void Book::setAuthor(const std::string &author) { author_ = author; }
void Book::setPages(int pages)
{
    if (validatePages(pages))
        pages_ = pages;
}

void Book::normalizeTitle()
{
    auto is_space = [](unsigned char ch)
    { return std::isspace(ch); };
    size_t start = 0;
    while (start < title_.size() && is_space(title_[start]))
        ++start;
    size_t end = title_.size();
    while (end > start && is_space(title_[end - 1]))
        --end;
    title_ = title_.substr(start, end - start);
}

bool Book::validatePages(int pages) const { return pages >= 0; }
