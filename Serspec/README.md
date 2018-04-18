## Serspec

Create a C++ application that parses a file similar to the attached xml file and gives back some information about the file content. 

The application can be either a console application or a GUI application. 
The example file contains a list of “**namedops**”; each namedop is a sequence of operations. An operation within a namedop can call a different namedop.

The application should print the answers to the following questions:
- How many namedops are in the file?

- Which **namedop** (identified by it’s “name” attribute) is the most complex? Which one is the east complex? Complexity is measured by the number of operations inside each **namedop**.

- Which **namedop** has a return value?

- Which is the maximum call depth for each **namedop**? A **namedop** that does not call another **namedop** has a call depth of 1.

- Are there any problems with the file? (the assumption is that the **.xml** file is always syntactically valid)
	
    **Answer**
  > some files (ex. web_interface.xml) can include circular calls (infinite calls).

