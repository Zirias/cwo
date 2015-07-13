# cwo -- C with objects

This library is just a toy project out of academic interest:

How far can you go creating an object-oriented type system in plain old C?

Of course, this will never be practical, let alone performant.

### Core types

Right now, the library defines some core types and 'bootstraps' them on 
initialization:

 - **cwo_Object**
   The base type for (nearly) everything. It is designed as a singleton, only
   one instance of an Object can exist and serves as the base instance for all
   objects that don't derive from another type. It provides basic type
   identification and cloning (flat).

 - **cwo_Type**
   Instances of this type represent a type. They have a unique name and an
   optional reference to a base type, as well as an optional type descriptor
   containing information about methods and properties.

 - **cwo_TypeDescriptor**
   Contains information about a type like a list of methods and properties
   defined by this type as well as a destructor reference.

 - **cwo_Method**
   Contains information about a method: name, number of arguments and the
   expected types of these. A method always returns `int`, this is for
   returning error codes (like it is common practice in C).

 - **cwo_Property**
   Describes a property with a name and a pair of getter/setter. Either of
   these can be `NULL`, making the property read- or write-only.

 - **cwo_String**
   A basic string class, right now quite dumb, just holds the content and
   length of a character array.

These core types have mutual dependencies, so they need 'bootstrapping' code.

