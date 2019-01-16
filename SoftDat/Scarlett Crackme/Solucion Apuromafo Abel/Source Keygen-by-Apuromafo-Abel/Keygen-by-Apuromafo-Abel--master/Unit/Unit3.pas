unit Unit3;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants,
  System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls;

type
  TForm3 = class(TForm)
    Edit1: TEdit;
    Edit2: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    Button1: TButton;
    Label3: TLabel;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
  { Public declarations }
    const
    dic1 = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ';
    dic2 = 'AHyukjsdfkjsdfnPQU5xWERY67345aq9nFyR';
    dic3 = 'o3zYzal1982Tv2FasgjkkjhkjlJt5Dpe32Ax';
  end;

var
  Form3: TForm3;

implementation

{$R *.dfm}

function HexStrToInt(const str: string): Integer;
begin
  Result := StrToInt('$' + str);
end;

function StringToHex(S: String): string;
var
  I: Integer;
begin
  Result := '';
  for I := 1 to length(S) do
    Result := Result + IntToHex(ord(S[I]), 2);
end;

procedure TForm3.Button1Click(Sender: TObject);
var
  nombre, nombremayus, nombredic2, serial: string;
  posicion, I, salto: Integer;
  // Form:TForm;
  serial2: string;
  apu1, apu2, apu3, salto2: Integer;
begin
  nombre := Edit1.Text;
  If length(nombre) < 3 then
  begin
    Showmessage('ingresar mínimo tres caracteres en nombre');
    Label3.Caption := 'ingresar minimo 3 carácteres ex: APU';
    Edit2.Text := ''
  end
  else
  begin
    { ShowMessage('comencemos has puesto '+inttostr(Length(nombre))+' caracteres') }
    Label3.Caption := 'Scarlet Crackme Keygen by Abel &Apuromafo';
    nombremayus := AnsiUpperCase(nombre);
    Edit2.Text := (nombremayus);
    { showmessage(nombremayus); }
    apu1 := length(nombremayus);
    { // probando si funcionaba con una constante con con variacion de salto
      case apu1 mod 2 of
      0:
      begin
      salto :=0;

      end;
      1:
      begin
      salto :=0;
      end;
      end;
    }
    salto := 0; { trabaja con salto 0 al comienzo }
    serial := '';
    // label3.caption:='salto es'+inttostr(salto);
    for I := 1 to apu1 do
    begin
      posicion := AnsiPos(nombremayus[I], dic1);
      { Showmessage(inttostr(posicion)); // verificando que posicion lee }
      if salto = 1 then
      begin
        nombredic2 := dic2[posicion];
        salto := salto - 1;
        serial := serial + nombredic2;
        // posicion:=0;
      end
      else
      begin
        nombredic2 := dic3[posicion];
        salto := salto + 1;
        serial := serial + nombredic2;
        // posicion:=0;
      end;
    end;

    apu2 := 0;
    case apu1 mod 2 of
      0:
        begin
          salto2 := 1;
        end;
      1:
        begin
          salto2 := 0;
        end;
    end;
    for I := 1 to apu1 do
    begin
      // showmessage(nombremayus[i])   ;
      apu3 := 0;
      apu3 := (((((HexStrToInt(StringToHex(nombremayus[I]))) xor salto2)
        shl $0A) xor $0A4ED0F7) - $29A);
      apu2 := apu2 + apu3;
    end;
    Edit2.Text := serial;
    // apu2:= apu2+apu2;
    // serial2:=serial+'-'+inttostr(apu2);
    // serial2:=serial+'-'+inttostr(apu2);
    if length(serial2) >= $018 then
    begin
      Edit2.Text := 'no valido  ';
      Label3.Caption := 'debes usar un nombre mas corto';
    end
    else
      Edit2.Text := Edit2.Text + '-' + inttostr(apu2)
      // serial2:=serial+'-'+inttostr(apu2);
  end;

end;

end.
