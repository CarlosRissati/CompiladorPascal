program TesteComErros;
var
  idade: integer;
  idade: real;  { ERRO: Variavel ja foi declarada. }
  nome: string;
  i: real;      { ERRO: Variavel 'i' para o FOR deve ser INTEGER }
begin
  salario := 1000.50; { ERRO: Variavel 'salario' nao foi declarada. }
  idade := 'trinta';   { ERRO: Incompatibilidade de tipos. }
  
  for i := 1 to 10 do { ERRO: 'i' é REAL }
  begin
    i := 5;            { ERRO: Nao pode modificar 'i' }
  end;
end.