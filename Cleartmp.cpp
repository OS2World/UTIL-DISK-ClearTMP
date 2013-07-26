#define INCL_VIO
#define INCL_DOS
#define INCL_DOSERRORS

#include <OS2.h>
#include <Stdio.h>
#include "FindRec.hpp"

// ��࠭� - �����.
#define RUSSIAN 866

// ��࠭�, � ���ன ࠡ�⠥� �ਫ������.
ULONG Code_page;

// �६���� ��⠫��.
PCHAR Temporary_directory;

// ��� ���⪠ �६����� ��⠫���� ���

VOID main( INT Parameters_count, CHAR* Parameters[] )
{
 // ������ ��࠭�, � ���ன ࠡ�⠥� �ਫ������.
 ULONG Size_of_list = 0;
 DosQueryCp( sizeof( Code_page ), &Code_page, &Size_of_list );

 // �᫨ ��ࠬ���� �� ������ - ᮮ�饭�� �� �⮬.
 if( Parameters_count < 2 )
  {
   // ���⠥� ᮮ�饭��.
   if( Code_page != RUSSIAN )
    {
     printf( "\nClearTMP is a tool for cleaning a temporary directories on a system startup." );
     printf( "\nVersion: 1.1" );
     printf( "\nUsage: ClearTMP <Temporary_directory>\n" );
    }
   else
    {
     printf( "\n�ਫ������ ClearTMP �।�����祭� ��� ���⪨ �६����� ��⠫���� �� ����㧪�" );
     printf( "\nOS/2. �ᯮ�짮�����: ClearTMP <����>" );
     printf( "\n�����: 1.1\n" );
    }

   // ������.
   return;
  }

 // �᫨ ����� ��� ����� 10 �㪢 - ������. ���� ������� "C:"?
 Temporary_directory = Parameters[ 1 ];
 if( strlen( Temporary_directory ) < 10 )
  {
   // ���⠥� ᮮ�饭��.
   if( Code_page != RUSSIAN ) printf( "\nA path length must be longer than 10 letters.\nNote: \"ClearTMP C:\" command is not allowed.\n" );
   else printf( "\n����� ��� ������ ���� ����� 10.\n�맮� \"ClearTMP C:\" �� ࠧ�襭.\n" );

   // ������.
   return;
  }

 // ���ࠥ� "\" � ���� ��ப�.
 INT Length = strlen( Temporary_directory );
 if( Temporary_directory[ Length - 1 ] == '\\' ) Temporary_directory[ Length - 1 ] = NULL;

 // ���⠥� ᮮ�饭��.
 if( Code_page != RUSSIAN ) printf( "Cleaning \"" );
 else printf( "���⪠ \"" );
 printf( Temporary_directory ); printf( "\" ...\n" );

 // ��頥� ��⠫��.
 FindFiles( Temporary_directory, 1 );

 // ������.
 return;
}

// ��� ��ࠡ��뢠�� 䠩� ���

VOID ProcessFile( PCHAR Name )
{
 // ��� �६���� ��⠫�� �ண��� �� �㦭�.
 if( strcmp( Name, Temporary_directory ) == EQUALLY ) return;

 // ����塞 䠩�.
 APIRET Report = DosForceDelete( Name );
 if( Report != NO_ERROR ) DosDeleteDir( Name );

 // ������.
 return;
}


