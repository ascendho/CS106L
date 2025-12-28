/*
 * CS106L Assignment 3: Make a Class
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 */

#include <iostream>
#include "class.h"

void sandbox()
{
  // Construct a default book
  Book defaultBook;

  // Construct a book using the custom constructor
  Book cppBook("  The C++ Programming Language  ", "Bjarne Stroustrup", 1376);

  // Demonstrate getters and that the title is normalized
  std::cout << "Default book title: " << defaultBook.getTitle() << "\n";
  std::cout << "CPP book title: " << cppBook.getTitle() << " by " << cppBook.getAuthor()
            << " (" << cppBook.getPages() << " pages)\n";

  // Use setters
  defaultBook.setTitle("A New Book");
  defaultBook.setAuthor("Student");
  defaultBook.setPages(123);

  std::cout << "After set -> " << defaultBook.getTitle() << " by " << defaultBook.getAuthor()
            << " (" << defaultBook.getPages() << " pages)\n";
}