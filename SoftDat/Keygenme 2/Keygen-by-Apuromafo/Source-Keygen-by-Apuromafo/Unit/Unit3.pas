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
    Label4: TLabel;
    Edit3: TEdit;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }

  end;

var
  Form3: TForm3;

implementation

{$R *.dfm}

// zona de funciones
function GetUserFromWindows: string;
var
  iLen: Cardinal;
begin
  iLen := 256; // UNLEN constant in LMCons.h says 256 - hard coded.
  Result := StringOfChar(#0, iLen);
  GetUserName(PChar(Result), iLen);
  SetLength(Result, iLen);
end;

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

// Fin zona funciones
procedure TForm3.Button1Click(Sender: TObject);
var
  nombre, nombre2, serial, serial2, serial3, serial4: string;
  apu2, apu3, apu4, I: Integer;

begin
  nombre := Edit1.Text;
  Label3.Caption := 'Keygen 2 by Apuromafo CLS';
  serial := '';
  Edit3.Text := '';
  apu2 := 0;
  apu3 := 0;
  I := 0;
  If length(nombre) < 1 then
  begin
    Showmessage('ingresar algun caracter porfavor');
    Label3.Caption := 'para el 2do algoritmo requiere mínimo 4 carácteres';
    Edit2.Text := ''
  end
  else
  begin
    for I := 0 to length(Edit1.Text) do
    begin
      apu3 := ((((HexStrToInt(StringToHex(nombre[I]))) + $1CB1) shl $9));
      apu3 := apu2 + apu3;
      apu3 := apu3 + (HexStrToInt(StringToHex(nombre[I]))) - $40;
      apu2 := apu3;
    end;
    serial2 := inttostr(apu2);
    Edit2.Text := serial2;
    If length(nombre) < 4 then
    begin
      Showmessage('ingresar algun caracter porfavor');
      Label3.Caption := 'mínimo 4 carácteres';
      Edit3.Text := ''
    end
    else
    begin
      Edit3.Text := GetUserFromWindows;
      nombre2 := Edit3.Text;
      apu2 := 0;
      apu3 := 0;
      apu4 := 0;
      I := 0;
      for I := 0 to length(Edit3.Text) do
      begin
        apu2 := ((((HexStrToInt(StringToHex(nombre2[I]))) * $7)));
        apu3 := apu2 + $F7C5;
        apu4 := apu3 + apu4;
      end;
      serial3 := '';
      serial3 := inttostr(apu4);
      nombre := Edit1.Text;
      apu2 := 0;
      apu3 := 0;
      apu4 := 0;
      I := 0;
      for I := 0 to length(Edit1.Text) do
      begin
        apu2 := ((HexStrToInt(StringToHex(nombre[5]))) * $5);
        apu3 := (HexStrToInt(StringToHex(nombre[2]))) * $144;
        apu4 := apu2 + apu3 + apu4;
      end;
      serial4 := '';
      serial4 := inttostr(apu4);
      Edit3.Text := 'A-' + nombre2 + '-B-' + serial3 + '-C-' + serial4;
      nombre2 := '';
      serial3 := '';
      serial4 := '';
      apu2 := 0;
      apu3 := 0;
      apu4 := 0;
      I := 0;
    end;
  end;
end;

end.
