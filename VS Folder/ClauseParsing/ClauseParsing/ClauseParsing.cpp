/*
  In this problem you will be implementing a theorem prover for a clause logic using the resolution
  principle***. Well-formed sentences in this logic are clauses. As mentioned in class, instead of
  using the implicative form, we will be using the disjunctive form, since this form is more suitable
  for automatic manipulation. The syntax of sentences in the clause logic is thus :

      Clause    ->  Literal v ... v Literal
      Literal   ->  ¬Atom | Atom
      Atom      ->  !True | False | P | Q | ...

  We will regard two clauses as identical if they have the same literals. For example, q v ¬p v q, q v ¬p, 
  and ¬p v q are equivalent for our purposes. For this reason, we adopt a standardized representation 
  of clauses, with duplicated literals always eliminated. When modeling real domains, clauses are often 
  written in the form :

      Literal ^ .. . ^ Literal -> Literal

  In this case, we need to transform the clauses such that they conform to the syntax of the clause
  logic. This can always be done using the following simple rules :

      1. (p -> q) is equivalent to (¬p v q)
      2. (¬(p v q)) is equivalent to(¬p ^ ¬q)
      3. (¬(p ^ q)) is equivalent to(¬p v ¬q)
      4. ((p ^ q) ^ ...) is equivalent to(p ^ q ^ ...)
      5. ((p v q) v ...) is equivalent to(p v q v ...)
      6. (¬(¬p)) is equivalent to p

      The proof theory of the clause logic contains only the resolution rule :

        ¬a v l1 v ... v ln,
         a v L1 v ... v Lm
        -------------------------
        l1 v ... v ln v L1 v ... v Lm

*** You need to be familiar with the components of a logic in order to solve this implementation assignment. If needed, read Chapters 7 and 9 of Russell and Norvig.

  If there are no literals l1,...ln and L1,...Lm, the resolution rule has the form :

      ¬a, a
      ------
      False

  Remember that inference rules are used to generate new valid sentences, given that a set of old sentences 
  are valid. For the clause logic this means that we can use the resolution rule to generate new valid clauses 
  given a set of valid clauses. Consider a simple example where p -> q, z -> y and p are valid clauses. To prove 
  that q is a valid clause we first need to rewrite the rules to disjunctive form : ¬p v q, ¬z v y and p. 
  Resolution is then applied to the first and last clause, and we get : 
  
      ¬p v q, p
      ---------
      q

  If False can be deduced by resolution, the original set of clauses is inconsistent. When making proofs by contradiction 
  this is exactly what we want to do. The approach is illustrated by the resolution principle explained below.
  
  
****** The Resolution Principle ******

  To prove that a clause is valid using the resolution method, we attempt to show that the negation of the clause 
  is unsatisfiable, meaning it cannot be true under any truth assignment. This is done using the following algorithm :

    1. Negate the clause and add each literal in the resulting conjunction of literals to the set of 
        clauses already known to be valid.
    2. Find two clauses for which the resolution rule can be applied. Change the form of the produced 
        clause to the standard form and add it to the set of valid clauses.
    3. Repeat 2 until False is produced, or until no new clauses can be produced. 
        If no new clauses can be produced, report failure; the original clause is not valid. 
        If False is produced, report success; the original clause is valid.

    Consider again our example. Assume we now want to prove that ¬z v y is valid. First, we negate the 
    clause and get z ^ ¬y. Then each literal is added to the set of valid clauses(see 4. and 5.). The 
    resulting set of clauses is :

        1. ¬p v q
        2. ¬z v y
        3. p
        4. z
        5. ¬y

  Resolution on 2. and 5. gives:

        1. ¬p v q
        2. ¬z v y
        3. p
        4. z
        5. ¬y
        6. ¬z

  Finally, we apply the resolution rule on 4. and 6. which produces False. Thus, the original clause ¬z v y is valid.


********** The Program **********

  Your program should read a text file containing the initial set of valid clauses and a clause for each literal 
  in the negated clause that we want to test validity of. Each line in the file defines a single clause. The literals of
  each clause are separated by a blank space and '~' is used to represent negation.

  Your program should implement the resolution algorithm as explained in the previous section. The output is either
  “Failure” if the clause cannot be shown to be valid, or the list of clauses in the proof tree for deducing False. 
  In either case you should also return the size of the final set of valid clauses.

  Let us consider a correct solution for testing the validity of ¬z v y for our example. The input file would be :

      ~p q
      ~z y
      p
      z
      ~y

  A possible final set of valid sentences could be :

      1. ~p q    {}
      2. ~z y    {}
      3. p       {}
      4. z       {}
      5. ~y      {}
      6. ~z      { 2,5 }
      7. False   { 4,6 }

  Note how the program keeps track of the parents of new clauses. This is used for extracting the clauses in the 
  proof tree. The solution you return consists of these clauses and the size of the final set of valid clauses :

      2. ~z y    {}
      4. z       {}
      5. ~y      {}
      6. ~z      { 2,5 }
      7. False   { 4,6 }
      Size of final clause set : 7


********** Power Plant Diagnosis **********

  In the last part of this assignment you will be using your resolution prover to verify the safety requirements 
  of a reactor unit in a nuclear power plant. The reactor unit is shown in the figure on the next page and consists 
  of a reactor R, two heat exchangers H1 and H2, two steam valves V 1 and V 2, and a control stick l for changing 
  the level of energy production. The state of the reactor unit is given by 5 propositional variables l, okH1, okH2,
  V 1 and V 2. If l has the value True the production level is 2 energy units. Otherwise, the production level is
  1 energy unit. At least one working heat exchanger is necessary for each energy unit produced to keep the reactor from 
  overheating. Unfortunately a heat exchanger i can start leaking reactive water from the internal cooling system to 
  the surroundings. okHi is False if heat exchanger Hi is leaking. Otherwise, okHi is True.When a heat exchanger i is 
  leaking, it must be shut off by closing its valve V i. The state variable V i indicates whether the valve V i is closed(False) 
  or open(True). Formally, the safety requirements are described by the following clauses :

          NoLeak ^ LowTemp      ->      ReactorUnitSafe
          NoLeakH1 ^ NoLeakH2   ->      NoLeak
          okH1                  ->      NoLeakH1
          ¬okH1 ^ ¬V1           ->      NoLeakH1
          okH2                  ->      NoLeakH2
          ¬okH2 ^ ¬V2           ->      NoLeakH2
          l ^ V1 ^ V2           ->      LowTemp
          ¬l ^ V1               ->      LowTemp
          ¬l ^ V2               ->      LowTemp

  Assume that the current state of the reactor unit is given by the clauses :
         
          ¬l
          ¬okH2
          okH1
          V 1
          ¬V 2

  1. Rewrite the safely rules from their implicative form to the disjunctive form used by your resolution prover. 
      The initial set of valid clauses is the union of the rule clauses and the clauses defining the current state. 
      Write the clauses in a file called facts.txt.
  
  2. Use your resolution prover to test whether LowTemp is a valid clause :
      (a)Save the input in a file called task1.in.
      (b)Save the result of your prover in a file called task1.out.
      [See Pic in PDF]
  
  3. Now test the validity of ReactorUnitSafe in a similar way :
      (a)Save the input in a file called task2.in.
      (b)Save the result of your prover in a file called task2.out.
  
  4. Consider a simpler set of safety rules :

          NoLeakH1 ^ NoLeakH2     ->      NoLeak
          okH1                    ->      NoLeakH1
          ¬okH1 ^ ¬V1             ->      NoLeakH1
          okH2                    ->      NoLeakH2
          ¬okH2 ^ ¬V2             ->      NoLeakH2

      and a reduced current state description :
 
          ¬okH2
          okH1
          ¬V 2

      Test the validity of ¬NoLeak :
          (a)Save the input in a file task3.in.
          (b)Save the result of your prover in a file called task3.out.

********** Requirements **********
The program must be written in C, C++, C#, Python, Visual Basic, or Java.If you want to use other programming languages, 
please contact your dear TA, Zach Stallbohm (zxs110030@utdallas.edu) for approval first. We strongly encourage you to use
built - in library functions to represent generic data structures such as sets, vectors, and lists. You must turn in documented 
source code, as well as a README file describing(i) which platform on which your code is developed(e.g., Windows / Linux / Solaris) 
and (ii)how to run your program. Basically, your program should take the name of the input file as a commandline 
argument(and no other arguments, please!). Also hand in a copy of the files mentioned in the power plant diagnosis exercises.

*/

#include "stdafx.h"

using namespace std;

struct Literal;
struct Clause;

vector<Clause*> originalClauses;
int clauseNum = 0;

struct Literal 
{
public:

  string name;
  bool negated = false;

  Literal(string n) 
  {
    if (negated = ( n[0] == '~') ) 
      name = n.substr(1, n.size()-1);
    else 
      name = n;
  }
};

struct Clause 
{
public:
  vector<Literal*> literals;
  string name;
  int ID;
  vector<Clause> parents;

  Clause(vector<Literal*> lits) 
  {
    ID = clauseNum;
    clauseNum++;

    literals = lits;
    
    SortLiterals();

    literals.erase(unique(literals.begin(), literals.end()), literals.end());

    CreateName();
  }

  Clause(vector<Literal*> lits, vector<Clause> p)
  {
    ID = clauseNum;
    clauseNum++;
    literals = lits;
    parents = p;

    SortLiterals();
    literals.erase(unique(literals.begin(), literals.end()), literals.end());
    CreateName();
  }

  // Negates the clause and adds each literal as a separate clause
  vector<Clause*> Negate()
  {
    vector<Clause*> negations;
    vector<Literal*> newLits = literals;

    for (int x = 0; x < newLits.size(); x++)
    {
      Literal* l = literals[x];
      l->negated = !l->negated;
      vector<Literal*> lits = { l };
      negations.push_back(new Clause(lits));
    }
    return negations;
  }

  // + Operator overloader. Essentially applies the resolution principle to the two clauses, and adds the result to the clauseVector
  Clause* operator+(const Clause& b) 
  {
    vector<Literal*> newL;

    // Combine the Clause's literal lists
    newL.reserve(literals.size() + b.literals.size());
    newL.insert(newL.end(), literals.begin(), literals.end());
    newL.insert(newL.end(), b.literals.begin(), b.literals.end());

    // Go through the list. Remove duplicates of the literal.
    // If there is a negated version of the same literal (I.E x31 and ~x31), remove both literals and go to the next literal
    int index = 0;
    while(index < newL.size())
    {
      Literal* l = newL[index];

      for (int x = index+1; x < newL.size(); x++) 
      {
        // Are they the same literal?
        if (l->name == newL[x]->name) 
        {
          // If it's an opposite literal, delete both, move to the next Literal
          if (l->negated != newL[x]->negated) 
          {
            newL.erase(newL.begin() + x);
            newL.erase(newL.begin() + index);
            index--;
            break;
          }
          // Otherwise it's a duplicate. Delete the duplicate and move on
          else 
          {
            newL.erase(newL.begin() + x);
            x--;
          }
        }
      }
      index++;
    }
    vector<Clause> parents = { *this, b };

    Clause* c = new Clause(newL, parents);
    
    if(newL.size() == 0)
      c->name = "False";

    return c;
  }

  // << Operator overloader. Prints out the clause
  friend ostream& operator<<(ostream& os, const Clause& c) 
  {
    os << c.ID << ". " << c.name << " {";

    if (c.parents.size() > 0)
    {
      for (int x = 0; x < c.parents.size() - 1; x++)
        os << c.parents[x].ID << ", ";

      os << c.parents[c.parents.size() - 1].ID;
    }

    os << "}";

    return os;
  }

  // Finds if this clause contains a given literal in its negated form
  bool ContainsNegatedLiteral(Literal* lit)
  {
    for (int x = 0; x < literals.size(); x++)
    {
      Literal* l = literals[x];
      if (l->name == lit->name && l->negated != lit->negated)
        return true;
    }
    return false;
  }

  void CreateName() 
  {
    name = "";

    for (int x = 0; x < literals.size(); x++)
    {
      if (literals[x]->negated)
        name += '~';
      name += literals[x]->name + " ";
    }
  }

  void SortLiterals()
  {
    for (int x = 0; x<literals.size(); x++)
    {
      int minI = x;

      for (int y = x; y<literals.size(); y++)
        if (literals[minI]->name > literals[y]->name)
          minI = y;

      Literal* temp = literals[x];
      literals[x] = literals[minI];
      literals[minI] = temp;
    }
  }
};

bool ContainsClause(Clause* c, vector<Clause*> v) 
{
  for (int x = 0; x < v.size(); x++) 
  {
    if (v[x]->name == c->name)
      return true;
  }
  return false;
}

void InitializeClauses(string fileName) 
{
  ifstream iFile;
  iFile.open(fileName);

  // Go line by line, split the string via white spaces, create new literals if needed. 
  // Make a new clause for each line.
  string line = "";
  while (getline(iFile, line))
  {
    //cout << "Line: " << line << endl;
    istringstream buf(line);
    istream_iterator<string> beg(buf), end;
    vector<string> tokens(beg, end); // done!

    vector<Literal*> lits;

    // Go through and add the literals as needed
    for (int x = 0; x < tokens.size(); x++)
      lits.push_back(new Literal(tokens[x]));

    Clause* c = new Clause(lits);

    originalClauses.push_back(c);
  }
}

Clause* ApplyResolution(Clause* cMain, Clause* cOther) 
{
  Clause* c = *cMain + *cOther;
  return c;
}

void SortClausesBySize() 
{
  for (int x = 0; x<originalClauses.size(); x++)
  {
    int minI = x;

    for (int y = x; y<originalClauses.size(); y++)
      if (originalClauses[minI]->literals.size() > originalClauses[y]->literals.size())
        minI = y;

    Clause* temp = originalClauses[x];
    originalClauses[x] = originalClauses[minI];
    originalClauses[minI] = temp;
  }
}

vector<Clause> SortClausesByID(vector<Clause> v)
{
  for (int x = 0; x<v.size(); x++)
  {
    int minI = x;

    for (int y = x; y<v.size(); y++)
      if (v[minI].ID > v[y].ID)
        minI = y;

    Clause temp = v[x];
    v[x] = v[minI];
    v[minI] = temp;
  }

  return v;
}

void PrintVector(vector<Clause*> v, string fileName) 
{
  Clause* currentClause = v[v.size() - 1];
  vector<Clause> clauses = 
  {
    *currentClause, 
    currentClause->parents[0], 
    currentClause->parents[1] 
  };

  bool noParents = false;
  int index = 1;
  while (index < clauses.size()) 
  {
    Clause c = clauses[index];

    if (c.parents.size() > 0) 
    {
      clauses.push_back(c.parents[0]);
      clauses.push_back(c.parents[1]);
    }

    index++;
  }

  clauses = SortClausesByID(clauses);

  ofstream output;
  output.open(fileName + ".out.txt");

  for (int x = 0; x < clauses.size(); x++) 
  {
    output << clauses[x] << endl;
    cout << clauses[x] << endl;
  }

  output << "Final Clause Size: " << v.size() << endl;
  cout << "Final Clause Size: " << v.size() << endl;
  
  output.close();
}

/*
void PartA(string fileName) 
{
  InitializeClauses(fileName);

  int index = 0;
  bool foundEmptyClause = false;

  vector <Clause*> clauses = originalClauses;
  Clause* clauseToProve = originalClauses[0];

  while (index < originalClauses.size())
  {
    cout << endl;

    foundEmptyClause = false;
    clauseToProve = originalClauses[index];
    Clause* result = NULL;
    clauses = originalClauses;
    clauses.erase(clauses.begin() + index);

    vector<Clause*> neg = clauseToProve->Negate();
    clauses.insert(clauses.end(), neg.begin(), neg.end());

    // Go through each clause
    // If you find a clause that can have resolution applied to it, apply it and add it to the clauses vector and continue to the next clause
    int i = 0;

    // Starting to go through the list to try and prove everything
    while (!foundEmptyClause && i <clauses.size())
    {
      Clause* currentClause = clauses[i];

      int a = i + 1;

      while (a < clauses.size())
      {
        Clause* compare = clauses[a];

        // Go through each literal
        for (int y = 0; y < currentClause->literals.size(); y++)
        {
          Literal* l = currentClause->literals[y];

          // If the compared clause contains a negated literal, apply resolution, move on to the next compare clause
          if (compare->ContainsNegatedLiteral(l))
          {
            result = ApplyResolution(currentClause, compare);

            if (!ContainsClause(result, clauses))
              clauses.push_back(result);

            break;
          }
        }

        if (result != NULL && result->literals.size() == 0)
        {
          result->name = "False";
          foundEmptyClause = true;
          break;
        }
        a++;
      }
      i++;
    }

    if (foundEmptyClause)
    {
      cout << endl;
      PrintVector(clauses);
      index++;
    }
    else
      break;
  }

  cout << "" << endl;

  if (!foundEmptyClause)
  {
    PrintVector(clauses);
    cout << "A contradiction has been found with " << *clauseToProve << endl;
  }
  else
  {
    PrintVector(originalClauses);
    cout << "The given clauses are valid" << endl;
  }
}
*/

void PartB(string file)
{
  string fileName = file;
  InitializeClauses(fileName);

  vector<Clause*> clauses = originalClauses;

  Clause* clauseToProve = clauses[clauses.size()-1];

  bool foundContradiction = false;
  int index = 0;
  while (index < clauses.size())
  {
    Clause* current = clauses[index];
    int i = index + 1;

    while (i < clauses.size())
    {
      Clause* compare = clauses[i];

      for (int x = 0; x < current->literals.size(); x++)
      {
        Literal* l = current->literals[x];
        if (compare->ContainsNegatedLiteral(l))
        {
          Clause* result = ApplyResolution(current, compare);
          clauses.push_back(result);
          break;
        }
      }

      if (foundContradiction = (clauses.back()->literals.size() == 0))
        break;

      i++;
    }

    if (foundContradiction)
      break;

    index++;
  }

  if (foundContradiction)
  {
    cout << "Found contradiction, " << clauseToProve->name << " is valid" << endl;
    string outFile = fileName.substr(0, 5);
    PrintVector(clauses, outFile);
  }

  else 
  {
    cout << "Did not find contradiction, " << clauseToProve->name << " is not valid" << endl;
    for (int x = 0; x < clauses.size(); x++)
      cout << *clauses[x] << endl;
  }
}

int main(int argc, char *argv[])
{


  PartB(argv[1]);
  cout << endl;

  return 0;
}
