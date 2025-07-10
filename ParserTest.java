public class ParserTest {
    public static void main(String[] args) throws IOException {
        // Test with your exemplo.txt file
        testFileParser();
        
        // Test with simple programs
        //testSimplePrograms();

        //testExhaustiveGrammarCoverage();

        //testEdgeCasePrograms();           // these new edge cases

        //testTypeAndRecordPrograms();

        //testRepeatUntilPrograms();

        //testForPrograms();

        //testLabelPrograms();

        //testConstPrograms();

        //testBuiltinPrograms();

        //testPrecedencePrograms();

        //testCaseOfPrograms();

        //testFileTypePrograms();

        //testBigguPrograms();
    }
    
    public static void testFileParser() throws IOException {
        System.out.println("=== Testing with exemplo.txt ===");
        try (var reader = new java.io.BufferedReader(new java.io.FileReader("exemplo.txt"))) {
            Lexer lexer = new Lexer(reader);
            List<Token> tokens = lexer.tokenize();
            
            if (lexer.hasErrors()) {
                System.err.println("Lexical errors:");
                for (Diagnostic err : lexer.getErrors()) {
                    System.err.println("  " + err);
                }
                return;
            }
            
            // Filter out ERROR tokens for parsing
            tokens.removeIf(token -> token.type == TK_Type.ERROR);
            
            Parser parser = new Parser(tokens);
            ProgramNode program = parser.parseProgram();
            
            if (parser.hasErrors()) {
                System.err.println("Parse errors:");
                for (Diagnostic diag : parser.getErrors()) {
                    System.err.println("  " + diag);
                }
            } else if (program != null) {
                System.out.println("Parse successful!");
                printAST(program, 0);
            }
        }
    }

        public static void testBigguPrograms() throws IOException {
        System.out.println("\n=== Testing File Type Parsing ===");

        String[] testPrograms = {
            // 1) ComplexExample
            """
            program ComplexExample;
            type
            IntArray = array[1..10] of integer;
            Person = record
                name: string;
                age: integer;
            end;
            PersonPtr = ^Person;

            var
            numbers: IntArray;
            p: PersonPtr;
            i: integer;

            procedure ProcessPerson(var person: Person);
            begin
            person.age := person.age + 1;
            end;

            function Calculate(x, y: integer): integer;
            begin
            Calculate := x * y + (x * 2);
            end;

            begin
            for i := 1 to 10 do
                numbers[i] := i * 2;
                
            case numbers[5] of
                1,2,3: writeln('small');
                10: writeln('ten');
            else
                writeln('other');
            end;
            
            New(p);
            p^.name := 'John';
            p^.age  := 30;
            ProcessPerson(p^);
            
            writeln('Success?');
            end.
            """,

            // 2) SetExamples
            """
            program SetExamples;

            type
                { Set type declarations }
                CharSet = set of char;
                DigitSet = set of 0..9;
                WeekdaySet = set of (Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday);
                
            var
                vowels: CharSet;
                digits: DigitSet;
                workdays: WeekdaySet;
                letters: set of 'A'..'Z';
                smallNumbers: set of 1..10;
                ch: char;
                num: integer;
                
            begin
                { Set constructor examples }
                vowels := ['a', 'e', 'i', 'o', 'u'];           { Explicit elements }
                digits := [0..9];                              { Range }
                letters := ['A'..'Z'];                         { Character range }
                workdays := [Monday, Tuesday, Wednesday, Thursday, Friday];
                smallNumbers := [1, 3, 5, 7, 9];             { Mixed explicit }
                
                { Empty set }
                vowels := [];
                
                { Complex set constructors }
                letters := ['A'..'M', 'N'..'Z'];              { Multiple ranges }
                digits := [0, 2, 4, 6, 8];                   { Even digits }
                vowels := ['a', 'e', 'i', 'o', 'u', 'A'..'E']; { Mixed elements and ranges }
                
                { IN operator usage }
                ch := 'a';
                if ch in vowels then
                    writeln('Character is a vowel');
                    
                num := 5;
                if num in [1..10] then
                    writeln('Number is between 1 and 10');
                    
                if ch in ['0'..'9'] then
                    writeln('Character is a digit');
                    
                { Set operations would be handled by operators +, -, * }
                { Union: set1 + set2 }
                { Difference: set1 - set2 }  
                { Intersection: set1 * set2 }
                
                { Nested set usage }
                if (ch in ['a'..'z']) and (ch in vowels) then
                    writeln('Lowercase vowel');
                    
                { Set comparison }
                if vowels = ['a', 'e', 'i', 'o', 'u'] then
                    writeln('Sets are equal');
                    
            end.
            """,
            // 3) MoarEdgeCases
            """
            program EdgeCaseTest;

            const
            C1       = 42;
            C2: Real = 3.14159;
            MsgText  = 'Don''t panic!';

            type
            Sub       = 1..5;
            ChSet     = set of 'A'..'Z';
            NumSet    = set of 0..9;

            R = record
                tag: Boolean;
                case Boolean of
                True:  (x: Integer);
                False: (y: Real);
            end;

            PR        = ^R;
            MyRec = record
                            tag2:   Char;
                            values: array[1..3] of Real;
                        end;

            var
            arr       : array[Sub] of Integer;
            rec       : R;
            prec      : PR;
            f         : file of Integer;
            chs       : ChSet;
            nums      : NumSet;
            flags     : set of Sub;
            myRecVar   : MyRec;
            resultVal : Real;
            piPtr     : ^Real;
            idx       : Sub;
            s         : string;

            function GetIndex(i: Integer): Sub;
            begin
            GetIndex := Sub((i mod C1) + 1);
            end;

            function SumValues(a, b: Integer): Integer;
            begin
            SumValues := a + b;
            end;

            begin
            // initialize array
            for idx := Low(Sub) to High(Sub) do
                arr[idx] := 0;

            // subrange constant usage
            idx := Sub((C1 mod 5) + 1);

            // pointer to real
            resultVal := 0.0;
            piPtr := @resultVal;
            piPtr^ := 2.71828;
            Writeln('piPtr^ = ', piPtr^:0:5);



            // array access, nested calls, squaring instead of **
            arr[GetIndex(idx)] := SumValues(arr[idx], Trunc(Sqr(Sin(3.14))));
            Writeln('arr[...] = ', arr[GetIndex(idx)]);

            // sets and membership
            chs   := ['A'..'M', 'X', 'Z'];
            nums  := [0, 2, 4..8];
            flags := [Low(Sub)..High(Sub)];
            if 'C' in chs then
                Writeln('C in chs');
            if idx in nums then
                Writeln('idx in nums');
            if idx in flags then
                Writeln('idx in flags');

            // variant record direct
            rec.tag := False;
            rec.y   := C2 * 2;
            if rec.y > C2 then
                Writeln(rec.y:0:3);

            // my record
            myRecVar.tag2      := 'P';
            myRecVar.values[2] := 5.5;
            Writeln('myRecVar.values[2] = ', myRecVar.values[2]:0:2);


            // file of integer
            Assign(f, 'out.bin');
            Rewrite(f);
            for idx := Low(Sub) to High(Sub) do
                Write(f, arr[idx]);
            Close(f);

            // string literal
            s := MsgText;
            Writeln(s);

            Dispose(prec);
            end.
            """,
            // 4) EvenMoarEdgeCases
            """
            program TestWithPacked;

            type
            // Simple record
            TPoint = record
                x, y: Integer;
            end;

            // Packed record (no padding)
            TRect = packed record
                left, top, right, bottom: Integer;
            end;

                // Packed variant record — the case header declares tag for you
                TVarRec = packed record
                case tag: Boolean of
                    True:  (iVal: Integer);
                    False: (rVal: Real);
                end;


            // Pointer to a packed record
            PPackedVarRec = ^TVarRec;

            // Packed one-dimensional array of variant records
            TPackedArr = packed array[1..3] of TVarRec;

            // Two-dimensional packed array of packed records
            TMatrix = packed array[1..2, 1..2] of packed record
                a, b: Integer;
            end;

            // Nested packed record example
            TNested = packed record
                inner: packed record
                m, n: Integer;
                end;
            end;

            var
            pt     : TPoint;
            rc     : TRect;
            vr     : TVarRec;
            vrp    : PPackedVarRec;
            parr   : TPackedArr;
            mat    : TMatrix;
            nest   : TNested;
            i, j   : Integer;

            procedure Demo;
            var
            // Local packed array of packed records
            localArr: packed array[1..2] of packed record
                v: Integer;
            end;
            tmpPtr: PPackedVarRec;
            begin
            // 1) simple WITH on a regular record
            with pt do
            begin
                x := 10;
                y := 20;
            end;

            // 2) nested WITH on the same variable
            with pt do
                with pt do
                x := x + y;

            // 3) WITH on a packed record
            with rc do
            begin
                left   := 0;
                top    := 0;
                right  := 100;
                bottom := 50;
            end;

            // 4) pointer to packed variant record + WITH
            New(vrp);
            vrp^.tag := True;
            vrp^.iVal := 1234;   // direct field before WITH
            with vrp^ do
            begin
                // switch to the other variant arm
                tag  := False;
                rVal := 3.14159;
            end;

            // 5) WITH on multiple variables at once
            with pt, rc do
            begin
                x := left + right;
                y := top + bottom;
            end;

            // 6) WITH on an element of a packed array
            for i := 1 to 3 do
            begin
                parr[i].tag := (i mod 2 = 0);
                parr[i].iVal := i * 10; // set integer side
                with parr[i] do
                begin
                if tag then
                    iVal := iVal * 2
                else
                    rVal := iVal / 2.0;
                end;
            end;

            // 7) WITH on a cell in a 2D packed array
            for i := 1 to 2 do
                for j := 1 to 2 do
                with mat[i, j] do
                begin
                    a := i * 10;
                    b := j * 100;
                end;

            // 8) Variant switching inside a WITH
            vr.tag := True;
            with vr do
            begin
                iVal := 42;
                tag := False;   // flip arm
                rVal := 6.28;
            end;

            // 9) Nested packed record + WITH
            nest.inner.m := 5;
            with nest do
                with inner do
                begin
                m := m + 1;
                n := m * 2;
                end;

            // 10) Local packed array + WITH
            for i := 1 to 2 do
                localArr[i].v := i * 7;
            with localArr[2] do
                v := v + 3;

            // 11) WITH inside a conditional
            if pt.x > 0 then
                with pt do
                writeln('pt in IF: (', x, ',', y, ')');

            // 12) WITH inside a REPEAT loop
            i := 3;
            repeat
                with pt do
                begin
                x := x - 1;
                y := y - 2;
                end;
                Dec(i);
            until i = 0;

            // 13) Combine pointer-to-packed and local pointer
            New(tmpPtr);
            tmpPtr^.tag := True;
            with tmpPtr^ do
            begin
                iVal := 777;
                tag := False;
                rVal := 9.99;
            end;
            Dispose(tmpPtr);
            end;

            begin
            Demo;

            writeln('Final pt = (', pt.x:0, ',', pt.y:0, ')');
            writeln('Final rc = (', rc.left:0, ',', rc.top:0, ',', rc.right:0, ',', rc.bottom:0, ')');
            writeln('Final vr (tag=', Ord(vr.tag), ') iVal=', vr.iVal, ' rVal=', vr.rVal:0:2);
            writeln('nest.inner = (', nest.inner.m:0, ',', nest.inner.n:0, ')');
            end.
            """,
            // 5) EdgeCaseExtraordinaire
            """
            program EdgeStressTest;

            type
            // A simple point record
            TPoint = record
                x, y: Integer;
            end;

            // A nested packed record inside a packed record
            TNested = packed record
                inner: packed record
                m, n: Integer;
                end;
            end;

            // A 2x2 matrix of points (packed)
            TMatrix = packed array[1..2, 1..2] of packed record
                x, y: Integer;
            end;

            // A “double-packed” array: array[1..2] of packed array[1..3] of packed record
            TDoubleRectArr = packed array[1..2] of packed array[1..3] of packed record
                left, top, right, bottom: Integer;
            end;

            var
            pt    : TPoint;
            nest  : TNested;
            mat   : TMatrix;
            darr  : TDoubleRectArr;
            i, j  : Integer;

            begin
            // 1) Mixed-level WITH: pt, then a matrix cell, then the nested record
            with pt, mat[i, j], nest.inner do
            begin
                // Here 'x,y' refer to pt.x/pt.y;
                x := i * 10;    
                y := j * 20;
                // 'x,y' inside mat[i,j] are the same names, so:
                mat[i, j].x := x + y;
                mat[i, j].y := x - y;
                // 'm,n' inside nest.inner
                m := x div 2;
                n := y div 2;
            end;

            // 2) Iterate the double-packed array and use WITH on each cell
            for i := 1 to 2 do
                for j := 1 to 3 do
                with darr[i, j] do
                begin
                    left   := i;
                    top    := j;
                    right  := i * 100;
                    bottom := j * 100;
                    writeln(
                    'darr[', i, ',', j, '] = (',
                    left:0, ',', top:0, ') -> (',
                    right:0, ',', bottom:0, ')'
                    );
                end;

            // 3) Nested WITH chains mixing all three at once
            i := 2; j := 1;
            with pt do
                with mat[i, j], nest.inner do
                begin
                x := left;      // left/top from darr would conflict—shows scoping rules
                y := top;       // but we're only with pt/mat/nest here
                m := x * y;
                n := x + y;
                end;

            writeln('Done stress-testing WITH and packed arrays!');
            end.
            """
        };

        for (int i = 0; i < testPrograms.length; i++) {
            System.out.println("\n--- Test FileType Program " + (i + 1) + " ---");
            testProgram(testPrograms[i]);
        }
    }

    public static void testFileTypePrograms() throws IOException {
        System.out.println("\n=== Testing File Type Parsing ===");

        String[] testPrograms = {
            // 1) Simple untyped file
            """
            program F1;
            var f: file;
            begin
            end.
            """,

            // 2) Typed file of integer
            """
            program F2;
            var f: file of integer;
            begin
            end.
            """,

            // 3) File in a type-alias declaration
            """
            program F3;
            type TF = file;
            var f: TF;
            begin
            end.
            """,

            // 4) File of an array type
            // ERROR
            // jdoodle.pas(2,28) Error: Type identifier expected
            // jdoodle.pas(2,28) Fatal: Syntax error, ";" expected but "ARRAY" found
            """
            program F4;
            var f: file of array[1..10] of char;
            begin
            end.
            """,

            // 5) File of a record type
            """
            program F5;
            type Rec = record
                x: integer;
            end;
            var f: file of Rec;
            begin
            end.
            """,

            // 6) Nested file-of-file
            """
            program F6;
            var f: file of file of integer;
            begin
            end.
            """,

            // 7) Missing OF keyword (should error)
            // ERROR
            // jdoodle.pas(2,25) Fatal: Syntax error, ";" expected but "identifier INTEGER" found
            """
            program F7;
            var f: file integer;
            begin
            end.
            """,

            // 8) Missing semicolon after var-decl (should error)
            // ERROR
            // jdoodle.pas(3,13) Fatal: Syntax error, ";" expected but "BEGIN" found
            """
            program F8;
            var f: file of integer
            begin
            end.
            """,

            // 9) OF without subtype (should error)
            // ERROR
            // jdoodle.pas(2,27) Error: Type identifier expected
            // jdoodle.pas(4,16) Fatal: There were 1 errors compiling module, stopping
            """
            program F9;
            var f: file of;
            begin
            end.
            """,

            // 10) Inline record after OF (should error)
            // ERROR
            // jdoodle.pas(2,28) Error: Type identifier expected
            // jdoodle.pas(2,28) Fatal: Syntax error, ";" expected but "RECORD" found
            """
            program F10;
            var f: file of record
                x: integer;
            end;
            begin
            end.
            """
        };

        for (int i = 0; i < testPrograms.length; i++) {
            System.out.println("\n--- Test FileType Program " + (i + 1) + " ---");
            testProgram(testPrograms[i]);
        }
    }

    public static void testCaseOfPrograms() throws IOException {
        System.out.println("\n=== Testing Precedence Rules ===");
        
        String[] testPrograms = {
            // 1) Original test
            """
            program P;
            var x: integer;
            begin
            x := 2;
            case x of
                1: writeln('one');
                2,3: writeln('two or three');
            else
                writeln('other');
            end;
            end.
            """,

            // 2) No semicolon after the last branch, no else
            """
            program P2;
            var x: integer;
            begin
            x := 1;
            case x of
                1: writeln('one');
                2: writeln('two')
            end;
            end.
            """,

            // 3) Semicolon after the last branch, no else
            """
            program P3;
            var x: integer;
            begin
            x := 2;
            case x of
                1: writeln('one');
                2: writeln('two');
            end;
            end.
            """,

            // 4) Omitting the semicolon before else
            """
            program P4;
            var x: integer;
            begin
            x := 3;
            case x of
                1: writeln('one');
                2: writeln('two')
            else
                writeln('other')
            end;
            end.
            """,

            // 5) With a ; after the else-statement
            """
            program P5;
            var x: integer;
            begin
            x := 4;
            case x of
                1: writeln('one');
                2: writeln('two')
            else
                writeln('other');
            end;
            end.
            """,

            // 6) Lots of extra semicolons
            // ERROR
            // jdoodle.pas(6,35) Error: Illegal expression
            // jdoodle.pas(7,17) Error: Constant Expression expected
            // jdoodle.pas(7,17) Fatal: Syntax error, ":" expected but "ordinal const" found
            """
            program P6;
            var x: integer;
            begin
            x := 5;
            case x of
                1: writeln('one');;
                2: writeln('two');;
            else;;
                writeln('other');;
            end;;
            end.
            """,

            // 7) Missing semicolon between branches
            // ERROR
            // jdoodle.pas(7,17) Fatal: Syntax error, ";" expected but "ordinal const" found
            """
            program P7;
            var x: integer;
            begin
            x := 1;
            case x of
                1: writeln('one')
                2: writeln('two')
            end;
            end.
            """,

            // 8) No semicolons anywhere
            // ERROR
            // jdoodle.pas(7,17) Fatal: Syntax error, ";" expected but "ordinal const" found
            """
            program P8;
            var x: integer;
            begin
            x := 1;
            case x of
                1: writeln('one')
                2: writeln('two')
            else
                writeln('other')
            end;
            end.
            """
        };
        
        for (int i = 0; i < testPrograms.length; i++) {
            System.out.println("\n--- Test Program " + (i + 1) + " ---");
            testProgram(testPrograms[i]);
        }
    }


    public static void testPrecedencePrograms() throws IOException {
        System.out.println("\n=== Testing Precedence Rules ===");
        
        String[] testPrograms = {
            // 1) * binds tighter than +, so 1 + 2*3 = 7
            """
            program Precedence1;
            var
                a: integer;
            begin
                a := 1 + 2 * 3;
                write(a)    // expect 7
            end.
            """,

            // 2) parentheses override: (1+2)*3 = 9
            """
            program Precedence2;
            var
                b: integer;
            begin
                b := (1 + 2) * 3;
                write(b)    // expect 9
            end.
            """,

            // 3) power is right-assoc: 2*32 means 2(32) = 2*9 = 512
            // ERROR
            // SEMATIC NO WORRIES
            """
            program PrecedencePower;
            var
                x: integer;
            begin
                x := 2 ** 3 ** 2;
                write(x)    // expect 512
            end.
            """,

            // 4) unary minus vs. multiplication: -1 * 2 = -2  (not -(1*2))
            """
            program PrecedenceUnary;
            var
                y: integer;
            begin
                y := -1 * 2;
                write(y)    // expect -2
            end.
            """,

            // 5) logical vs. relational vs. arithmetic:
            //    1+2=3 AND NOT FALSE  →  TRUE AND TRUE  → TRUE
            // ERROR
            // SEMATIC NO WORRIES
            """
            program PrecedenceLogical;
            var
                ok: boolean;
            begin
                ok := 1 + 2 = 3 AND NOT FALSE;
                write(ok)  // expect TRUE
            end.
            """,

            // 6) mixing record field and pointer dereference with arithmetic:
            """
            program PrecedenceRecordPtr;
            type
                TRec = record
                    f: integer;
                end;
                PRec = ^TRec;
            var
                r: TRec;
                p: PRec;
                val: integer;
            begin
                r.f := 2;
                new(p);
                p^.f := 3;
                // (r.f + p^.f)*2 = (2+3)*2 = 10
                val := (r.f + p^.f) * 2;
                write(val);   // expect 10
                dispose(p)
            end.
            """
        };
        
        for (int i = 0; i < testPrograms.length; i++) {
            System.out.println("\n--- Test Program " + (i + 1) + " ---");
            testProgram(testPrograms[i]);
        }
    }

    public static void testBuiltinPrograms() throws IOException {
        System.out.println("\n=== Testing Built-in Procedures ===");
        
        String[] testPrograms = {
            // 1) Simple read/write
            """
            program ReadWriteSimple;
            var
                x: integer;
            begin
                read(x);
                write(x)
            end.
            """,

            // 2) readln and writeln with multiple values
            """
            program ReadLnMultiple;
            var
                a, b: integer;
            begin
                readln(a, b);
                writeln(a, ',', b)
            end.
            """,

            // 3) Echo a string
            """
            program EchoString;
            var
                s: string;
            begin
                readln(s);
                writeln(s)
            end.
            """,

            // 4) writeln only
            """
            program HelloWorld;
            begin
                writeln('Hello, world!');
                writeln('This is a test.')
            end.
            """,

            // 5) Simple new/dispose for a single integer
            """
            program DynamicInteger;
            type
                PInt = ^Integer;
            var
                p: PInt;
            begin
                new(p);
                p^ := 123;
                write(p^);
                dispose(p)
            end.
            """,

            // 6) new/dispose an array of 3 integers
            // ERROR
            // jdoodle.pas(8,26) Fatal: Syntax error, "identifier" expected but "ordinal const" found
            """
            program DynamicArray;
            type
                TIntArr = array[1..3] of Integer;
                PArr = ^TIntArr;
            var
                arr: PArr;
            begin
                new(arr, 3);
                arr^[1] := 10;
                arr^[2] := 20;
                arr^[3] := 30;
                writeln(arr^[1], ',', arr^[2], ',', arr^[3]);
                dispose(arr, 3)
            end.
            """,

            // 7) Mixed I/O and dynamic memory
            """
            program MixIOAndMemory;
            type
                PInt = ^Integer;
            var
                x: Integer;
                p: PInt;
            begin
                write('Enter a number: ');
                readln(x);
                new(p);
                p^ := x * x;
                writeln('Square = ', p^);
                dispose(p)
            end.
            """,

            // 8) Error case: dispose without new
            """
            program DisposeError;
            var
                p: ^Integer;
            begin
                dispose(p)   // error: p was never allocated
            end.
            """
        };
        
        for (int i = 0; i < testPrograms.length; i++) {
            System.out.println("\n--- Test Program " + (i + 1) + " ---");
            testProgram(testPrograms[i]);
        }
    }


    public static void testConstPrograms() throws IOException {
        System.out.println("\n=== Testing Const Programs ===");
        
        String[] testPrograms = {
            // 1) Simple untyped constant
            """
            program ConstSimple;
            const PI = 3.14;
                    Neg = -5;
            var x: real;
            begin
                x := PI;
                write(x)
            end.
            """,

            // 2) Typed constant
            """
            program ConstTyped;
            const Max: integer = 10;
            var i: integer;
            begin
                i := Max;
                write(i)
            end.
            """,

            // 3) Multiple constants in one section
            // ERROR
            // jdoodle.pas(5,26) Error: Illegal expression
            // jdoodle.pas(8,39) Error: Identifier not found "C"
            // jdoodle.pas(9,16) Fatal: There were 2 errors compiling module, stopping
            """
            program ConstMultiple;
            const
                A = 1;
                B: integer = A + 2;
                C = B * 2;
            var x: integer;
            begin
                write(A, ',', B, ',', C)
            end.
            """,

            // 4) Boolean and string constants
            """
            program ConstBoolString;
            const
                Flag = true;
                Greeting: string = 'Hello, world!';
            var ok: boolean;
            begin
                ok := Flag;
                write(Greeting);
                write(ok)
            end.
            """,

            // 5) Constant used as a parameter
            """
            program ConstParam;

            var res: integer;

            function Double(const N: integer): integer;
            begin
                Double := N * 2
            end;
            
            begin
                res := Double(5);
                write(res)
            end.
            """,

            // 6) Mixing const with label/type/var sections
            """
            program MixSections;
            label 100;
            const Threshold = 8;
            type T = integer;
            var v: T;
            begin
                v := Threshold;
            100: write(v)
            end.
            """,

            // 7) Error: using ':=' instead of '=' in const
            // ERROR
            // jdoodle.pas(2,30) Fatal: Syntax error, "=" expected but ":=" found
            """
            program ConstError1;
            const Y: integer := 20;    // should be '=' not ':='
            var z: integer;
            begin
                z := Y;
                write(z)
            end.
            """,

            // 8) Error: missing initializer
            // ERROR
            // jdoodle.pas(2,29) Fatal: Syntax error, "=" expected but ";" found
            """
            program ConstError2;
            const M: integer;          // no '= value' -> syntax error
            var a: integer;
            begin
                a := M;
                write(a)
            end.
            """
        };
        
        for (int i = 0; i < testPrograms.length; i++) {
            System.out.println("\n--- Const Test Program " + (i + 1) + " ---");
            testProgram(testPrograms[i]);
        }
    }


    public static void testLabelPrograms() throws IOException {
        System.out.println("\n=== Testing Label/Goto Programs ===");
        
        String[] testPrograms = {
            // 1) Simple numeric label and backward goto
            """
            program GotoSimple;
            label 100;
            var x: integer;
            begin
                x := 0;
            100: x := x + 1;
                if x < 3 then
                    goto 100;
                write(x)
            end.
            """,

            // 2) Identifier label, forward and backward hops
            // ERROR
            // jdoodle.pas(6,22) Error: Identifier not found "SkipInit"
            // jdoodle.pas(6,30) Error: identifier isn't a label
            // jdoodle.pas(9,13) Error: Identifier not found "SkipInit"
            // jdoodle.pas(9,21) Fatal: Syntax error, ";" expected but ":" found
            """
            program GotoIdent;
            label Start;
            var count: integer;
            begin
                count := 1;
                goto SkipInit;
            Start:
                count := count + 1;
            SkipInit:
                if count <= 5 then
                    goto Start;
                write(count)
            end.
            """,

            // 3) Multiple labels declared together, mixing numbers and names
            """
            program GotoMultiple;
            label L1, L2, 200;
            var i: integer;
            begin
                i := 1;
            L1:
                write('Loop #', i);
                i := i + 1;
                if i <= 3 then goto L1;
            200:
                write(' Now jump to named label');
                goto L2;
            L2:
                write('Done')
            end.
            """,

            // 4) Forward goto jumping into a nested compound statement
            """
            program ForwardIntoBlock;
            label SkipBlock;
            var a: integer;
            begin
                a := 42;
                goto SkipBlock;
                begin
                    write('This should be skipped');
                    a := 0;
                end;
            SkipBlock:
                write(a)
            end.
            """,

            // 5) Edge: label declared but never used, and goto to undeclared label
            // ERROR
            // SEMANTIC
            // jdoodle.pas(6,22) Error: Identifier not found "Missing"
            // jdoodle.pas(6,29) Error: identifier isn't a label
            // jdoodle.pas(8,16) Fatal: There were 2 errors compiling module, stopping
            """
            program UnusedAndBadGoto;
            label UnusedLabel;
            var x: integer;
            begin
                x := 5;
                goto Missing;    // parser should flag Missing as error
                write(x);
            end.
            """,

            // 6) Mixing labeled and unlabeled statements without a LABEL section
            // ERROR
            // jdoodle.pas(4,20) Error: Label used but not defined "1"
            // jdoodle.pas(4,20) Fatal: Syntax error, ";" expected but "identifier I" found
            """
            program ImplicitLabels;
            var i: integer;
            begin
                1: i := 1;      // a label without declaration section
                write(i);
                goto 1;
            end.
            """,

            // 7) Double or nested labels in a statement
            """
            program DoubletLabels;
            label L1, L2;
            var i: integer;
            begin
                L2: L1: i := 1;      // a label without declaration section
                write(i);
            end.
            """
        };
        
        for (int i = 0; i < testPrograms.length; i++) {
            System.out.println("\n--- Label Test Program " + (i + 1) + " ---");
            testProgram(testPrograms[i]);
        }
    }


    public static void testForPrograms() throws IOException {
        System.out.println("\n=== Testing For-Loop Programs ===");
        
        String[] testPrograms = {
            // 1) Simple ascending for
            """
            program ForSimple;
            var i: integer;
            begin
                for i := 1 to 5 do
                    write(i)
            end.
            """,

            // 2) Summation with compound body
            """
            program ForSum;
            var i, sum: integer;
            begin
                sum := 0;
                for i := 1 to 10 do
                begin
                    sum := sum + i;
                end;
                write(sum)
            end.
            """,

            // 3) Descending loop (DOWNTO)
            """
            program ForDown;
            var j: integer;
            begin
                for j := 10 downto 1 do
                    write(j);
            end.
            """,

            // 4) Nested loops
            """
            program ForNested;
            var i, j: integer;
            begin
                for i := 1 to 3 do
                begin
                    for j := 1 to 2 do
                        write('(', i, ',', j, ')');
                    writeln;
                end;
            end.
            """,

            // 5) Loop with array indexing
            """
            program ForArray;
            var a: array[1..5] of integer;
                i: integer;
            begin
                for i := 1 to 5 do
                    a[i] := i * 2;
                for i := 1 to 5 do
                    write(a[i]);
            end.
            """
        };
        
        for (int i = 0; i < testPrograms.length; i++) {
            System.out.println("\n--- For Test Program " + (i + 1) + " ---");
            testProgram(testPrograms[i]);
        }
    }


    public static void testRepeatUntilPrograms() throws IOException {
        System.out.println("\n=== Testing REPEAT & UNTIL Support ===");
        Map<String,String> cases = Map.ofEntries(
            // 1. Simple type alias
            Map.entry("Counting from 1 to 5", """
                program CountToFive;
                var
                i: integer;
                begin
                i := 1;
                repeat
                    writeln(i);
                    i := i + 1;
                until i > 5;
                end.
                """),
            Map.entry("SumUntilZero", """
                program SumUntilZero;
                var
                x, sum: integer;
                begin
                sum := 0;
                repeat
                    write('Enter a number (0 to stop): ');
                    readln(x);
                    sum := sum + x;
                until x = 0;
                writeln('Total sum: ', sum);
                end.
                """),
            Map.entry("ValidateGrade", """
                program ValidateGrade;
                var
                grade: integer;
                begin
                repeat
                    write('Enter a grade (0-100): ');
                    readln(grade);
                    if (grade < 0) or (grade > 100) then
                    writeln('  >> Invalid. Please try again.');
                until (grade >= 0) and (grade <= 100);
                writeln('You entered: ', grade);
                end.
                """),
            Map.entry("SumEvenTen", """
                program SumEvenTen;
                var
                count, n, sum: integer;
                begin
                count := 0;
                n := 0;
                sum := 0;
                repeat
                    n := n + 1;
                    if (n mod 2 = 0) then
                    begin
                    sum := sum + n;
                    count := count + 1;
                    end;
                until count = 10;
                writeln('Sum of first 10 even numbers is: ', sum);
                end.
                """),
            // ERROR
            // SEMANTIC
            // jdoodle.pas(10,35) Error: Identifier not found "i"
            // Jdoodle.pas(11,20) Fatal: There were 1 errors compiling module, stopping
            Map.entry("arrTest", """
                program ArrTest;
                TYPE
                    IntArray = array[1..10] of integer;  // <-- ArrayTypeNode handles this

                VAR
                    myArray: IntArray;

                BEGIN
                    myArray[5] := 42;  // <-- This access needs a new expression node
                    WRITE(myArray[i]); // <-- And this too
                END.
                """),
            Map.entry("RepeatWithFlag", """
                program RepeatWithFlag;
                var
                i: integer;
                stop: boolean;
                begin
                i := 0;
                stop := false;
                repeat
                    write('Iteration ', i+1, '. Enter a number: ');
                    readln(i);
                    if i < 0 then
                    stop := true;
                    i := i + 1;
                until (i >= 5) or stop;
                if stop then
                    writeln('Stopped early due to negative input.')
                else
                    writeln('Completed 5 iterations.');
                end.
                """)
        );

        for (var entry : cases.entrySet()) {
            System.out.printf("\n--- %s ---\n", entry.getKey());
            try {
                testProgram(entry.getValue());
            } catch (RuntimeException e) {
                System.err.println("  ** Expected error: " + e.getMessage());
            }
        }
    }

    public static void testTypeAndRecordPrograms() throws IOException {
        System.out.println("\n=== Testing TYPE & RECORD Support ===");
        Map<String,String> cases = Map.ofEntries(
            // 1. Simple type alias
            Map.entry("Type_Alias_Simple", """
                program TA;
                type
                TInt = integer;
                var
                x: TInt;
                begin end.
                """),

            // 2. Multiple aliases in one section
            Map.entry("Type_MultiAlias", """
                program TMA;
                type
                TInt = integer;
                TReal = real;
                TBool = boolean;
                var
                a: TInt;
                b: TReal;
                c: TBool;
                begin end.
                """),

            // 3. Simple record with two fields
            Map.entry("Record_Simple", """
                program RS;
                type
                Point = record
                    x, y: real;
                end;
                var
                p: Point;
                begin end.
                """),

            // 4. Nested record types
            Map.entry("Record_Nested", """
                program RN;
                type
                Inner = record
                    a: Integer;
                    b: record
                        first, middle : string;
                        age, whatever : integer;
                    end;
                end;

                Outer = record
                    innerRec: Inner;   { ← renamed from “in” to avoid the reserved word }
                    flag: Boolean;
                end;
                var
                o: Outer;
                begin end.
                """),

            // 5. Array of record
            Map.entry("ArrayOfRecord", """
                program AOR;
                type
                Person = record
                    name: char;
                    age: integer;
                end;
                var
                roster: array[1..5] of Person;
                begin end.
                """),

            // 6. Named type before alias (forward reference)
            // ERROR
            // jdoodle.pas(3,23) Error: Identifier not found "TNum"
            // jdoodle.pas(3,27) Error: Error in type definition
            // jdoodle.pas(6,26) Fatal: There were 2 errors compiling module, stopping
            Map.entry("NamedType_BeforeDecl", """
                program NFD;
                var
                temp: TNum;
                type
                TNum = integer;
                begin end.
                """),

            // 7. Using alias in function return type
            Map.entry("Func_UsingAlias", """
                program FUA;
                type
                TReal = real;
                function f(x: integer): TReal;
                begin
                f := x * 1.5
                end;
                begin end.
                """),

            // 8. Empty record (no fields)
            Map.entry("Record_Empty", """
                program RE;
                type
                Empty = record
                end;
                var
                e: Empty;
                begin end.
                """),

            // 9. Error: missing END on record
            // ERROR
            // jdoodle.pas(5,17) Fatal: Syntax error, "END" expected but "VAR" found
            Map.entry("Record_MissingEnd_Invalid", """
                program RME;
                type
                BadRec = record
                    a: integer;
                var x: BadRec;
                begin end.
                """),

            // 10. Error: undefined named type
            // ERROR
            // jdoodle.pas(3,20) Error: Identifier not found "UnknownType"
            // jdoodle.pas(3,31) Error: Error in type definition
            // jdoodle.pas(4,26) Fatal: There were 2 errors compiling module, stopping
            Map.entry("NamedType_Undefined_Invalid", """
                program NUI;
                var
                x: UnknownType;
                begin end.
                """)
        );

        for (var entry : cases.entrySet()) {
            System.out.printf("\n--- %s ---\n", entry.getKey());
            try {
                testProgram(entry.getValue());
            } catch (RuntimeException e) {
                System.err.println("  ** Expected error: " + e.getMessage());
            }
        }
    }

    
    public static void testSimplePrograms() throws IOException {
        System.out.println("\n=== Testing Simple Programs ===");
        
        String[] testPrograms = {
            // Simple program
            """
            program Hello;
            begin
                write('Hello World');
                write('Hello World');
                write('Hello World');
            end.
            """,
            
            // Program with variables
            """
            program Test;
            var x, y: integer;
                z: real;
            begin
                x := 10;
                y := x + 5;
                write(x, y)
            end.
            """,
            
            // Program with procedure
            """
            program TestProc;
            var n: integer;
            
            procedure greet;
            begin
                write('Hello')
            end;
            
            begin
                read(n);
                greet
            end.
            """
        };
        
        for (int i = 0; i < testPrograms.length; i++) {
            System.out.println("\n--- Test Program " + (i + 1) + " ---");
            testProgram(testPrograms[i]);
        }
    }
    
    private static void testProgram(String program) throws IOException {
        StringReader stringReader = new StringReader(program);
        Lexer lexer = new Lexer(stringReader);
        List<Token> tokens = lexer.tokenize();
        
        if (lexer.hasErrors()) {
            System.err.println("Lexical errors:");
            for (Diagnostic err : lexer.getErrors()) {
                System.err.println("  " + err);
            }
            return;
        }
        
        // Filter out ERROR tokens
        tokens.removeIf(token -> token.type == TK_Type.ERROR);
        
        Parser parser = new Parser(tokens);
        ProgramNode programNode = parser.parseProgram();
        
        if (parser.hasErrors()) {
            System.err.println("Parse errors:");
            for (Diagnostic diag : parser.getErrors()) {
                System.err.println("  " + diag);
            }
        } else if (programNode != null) {
            System.out.println("✓ Parse successful!");
            printAST(programNode, 0);
        }
    }

    public static void testEdgeCasePrograms() throws IOException {
        System.out.println("\n=== Edge-Case & Extra Tests ===");

        Map<String, String> cases = Map.ofEntries(
            // 1. Multi-statement THEN/ELSE
            Map.entry("If_MultiStatement", """
                program MTE;
                var Cond: boolean;
                    x, y, z: integer;
                begin
                    if Cond then
                        begin x := 1; y := 2 end
                    else
                        begin z := 3; x := 4 end
                end.
                """),

            // 2. Procedure calls with/without parameters
            Map.entry("ProcCall_NoArgs", """
                program PC;
                procedure foo; begin end;
                begin
                    foo;
                end.
                """),
            Map.entry("ProcCall_EmptyParens", """
                program PC2;
                procedure foo; begin end;
                begin
                    foo();
                end.
                """),
            Map.entry("ProcCall_ManyArgs", """
                program PC3;
                procedure bar(a: integer; b: real); begin end;
                begin
                    bar(10, 3.14);
                end.
                """),

            // 3. Function with multiple return assignments
            Map.entry("Func_MultiReturn", """
                program MR;
                function f(x: integer): integer;
                begin
                    f := x * 2;
                    f := f + 1
                end;
                begin end.
                """),

            // 4. Mixed VAR sections (spacing & casing)
            Map.entry("Var_Uppercase", """
                program MV1;
                VAR A, B: INTEGER;
                begin end.
                """),
            Map.entry("Var_LotsOfSpace", """
                program MV2;
                var    x   :    real   ;
                begin end.
                """),
            Map.entry("Var_MixedCase", """
                program MV3;
                Var c, d, e: Boolean;
                Begin End.
                """),

            // 5. Error cases
            Map.entry("ExtraSemicolons_Invalid", """
                program ES;
                var x: integer;;;
                begin;;; x := 1;;;
                end;;.
                """),
            Map.entry("MissingSemicolon_Invalid", """
                program MS;
                var x: integer
                begin x := 1 end.
                """),
            Map.entry("BadCommasInParams_Invalid", """
                program BC;
                procedure p(a,,b: integer); begin end;
                begin end.
                """),
            Map.entry("UnterminatedString_Invalid", """
                program US;
                begin
                    write('oops)
                end.
                """),
            Map.entry("UnterminatedComment_Invalid", """
                program UC;
                { this comment never ends…
                begin x := 1; end.
                """),

            // 6. Nested compound and loops
            Map.entry("Nested_WhileInThen", """
                program NCS;
                var x: integer;
                begin
                    if x > 0 then
                        begin
                            while x < 10 do begin x := x + 1 end
                        end
                end.
                """),

            // 7. Nested function calls in expressions
            Map.entry("NestedFuncCalls", """
                program NC;
                function g(a: integer): integer; begin g := a * 2 end;
                function f(x, y: integer): integer; begin f := x + y end;
                begin
                    write(f(g(1), g(2)));
                end.
                """),

            // 8. Unary NOT vs AND precedence
            Map.entry("UnaryNot_Precedence", """
                program UNP;
                var a, b: boolean;
                begin
                    x := not a and b;
                end.
                """)
        );

        for (var entry : cases.entrySet()) {
            System.out.printf("\n--- %s ---\n", entry.getKey());
            try {
                testProgram(entry.getValue());
            } catch (RuntimeException e) {
                System.err.println("  ** Expected error: " + e.getMessage());
            }
        }
    }


    public static void testExhaustiveGrammarCoverage() throws IOException {
        System.out.println("\n=== Exhaustive Grammar Coverage ===");

        // Map each test name to its program text
        Map<String, String> cases = Map.ofEntries(
            Map.entry("Program_Minimal", """
                program P;
                begin end.
                """),
            // ERROR
            Map.entry("Program_MissingDot_Invalid", """
                program P;
                begin end
                """),
            Map.entry("Var_Single", """
                program V; var x: integer; begin x := 0 end.
                """),
            Map.entry("Var_Multiple", """
                program V2;
                var a, b: integer;
                    c: real;
                begin end.
                """),
            // ERROR
            Map.entry("Var_TrailingComma_Invalid", """
                program X; var a,: integer; begin end.
                """),
            Map.entry("Proc_Simple", """
                program Pr; procedure p; begin end; begin end.
                """),
            Map.entry("Func_WithReturn", """
                program Fn;
                function f(x: integer): real;
                begin f := x * 1.0 end;
                begin end.
                """),
            Map.entry("FormalParams_Mixed", """
                program M;
                procedure q(a,b: integer; c: real);
                begin end;
                begin end.
                """),
            Map.entry("FormalParams_Empty", """
                program Emp; procedure e(); begin end; begin end.
                """),
            Map.entry("If_ThenElse", """
                program If1;
                var x: integer;
                begin
                    if x > 0 then write(x) else write(-x)
                end.
                """),
            // ERROR
            Map.entry("If_MissingThen_Invalid", """
                program If2;
                begin if x = 1 write(x) end.
                """),
            Map.entry("While_Simple", """
                program Wh; var x: integer;
                begin
                    while x <> 0 do x := x - 1
                end.
                """),
            // ERROR
            Map.entry("While_MissingDo_Invalid", """
                program W2; begin while x < 5 x := x + 1 end.
                """),
            Map.entry("ReadWrite_Combined", """
                program IO; var a,b: integer;
                begin
                    read(a,b);
                    write(a + b, 'done')
                end.
                """),
            Map.entry("NestedArrays", """
                program A1;
                var arr: array[1..2] of array[1..3] of real;
                begin end.
                """),
            Map.entry("Expression_Complex", """
                program E;
                var x,y,z: integer;
                begin
                    x := -y + (z * (x mod 2)) or not y
                end.
                """),
            // ERROR
            Map.entry("BadParens_Invalid", """
                program P; begin write((1+2) end.
                """)
            // … add more entries to cover every production …
        );

        for (var entry : cases.entrySet()) {
            System.out.printf("\n--- %s ---\n", entry.getKey());
            try {
                testProgram(entry.getValue());
            } catch (RuntimeException e) {
                System.err.println("  ** Expected error: " + e.getMessage());
            }
        }
    }
}