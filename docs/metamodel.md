# The metamodel used by Fin

The following code is the textual view of OCLinEcore over the ecore metamodel.

```java
package model : fin = 'http://www.ensta-bretagne.fr/fin'
{
	abstract class LocatedElement
	{
		attribute line : Integer[1];
		attribute offset : Integer[1];
		attribute startIndex : Integer[1];
		attribute stopIndex : Integer[1];
	}
	abstract class NamedElement extends LocatedElement
	{
		attribute name : String[1];
	}
	class AgentDeclaration extends NamedElement
	{
		attribute ports : String[*];
	}
	abstract class Net extends LocatedElement;
	class Name extends Net,NamedElement;
	class AgentInstance extends Net,NamedElement
	{
		property connections : Net[*];
	}
	class Equation extends Net
	{
		property lhs : Net[1];
		property rhs : Net[1];
	}
	class NetList extends Net
	{
		property components : Net[*];
	}
	class ActivePair extends Equation;
	class Rule extends LocatedElement
	{
		property activePair : ActivePair[1];
		property target : Net[1];
	}
	class NetDeclaration
	{
		property agents : AgentDeclaration[*];
		property rules : Rule[*];
		property net : Net[1];
	}
}
```