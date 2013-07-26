#define INCL_DOSFILEMGR
#define INCL_DOSERRORS

#include <OS2.h>
#include <Stdlib.h>
#include <String.h>

// �������� ������.
#define CONST   const
#define STATIC  static
#define FLOAT   float
#define EQUALLY 0

// ����� ��ப� ��� ����� 䠩��.
#define SIZE_OF_NAME   255
#define SIZE_OF_PATH  1024

// �믮���� ���� 䠩���.
VOID FindFiles( PCHAR, BYTE );
// ���⠢��� ������ ��� 䠩��.
VOID MakePathname( PCHAR, PCHAR, PCHAR );

// ��ࠡ��뢠�� 䠩�. ������ ���� ��ॣ�㦥�� ���짮��⥫��.
VOID ProcessFile( PCHAR );

// ��� �믮���� ���� 䠩��� ���

// Path - ����, � ���ண� ���� ����� ����, Include_dirs - ���� �� ��ࠡ��뢠�� ��⠫���.
VOID FindFiles( PCHAR Path, BYTE Include_dirs )
{
 // ����騩 ����.
 STATIC CHAR Static_path[ SIZE_OF_PATH ];

 // ������ ����� ��� ���᪠.
 CHAR* Path_mask = new CHAR[ strlen( Path ) + SIZE_OF_NAME ];
 if( Path_mask == NULL ) return;

 if( Path[ 0 ] == NULL ) strcpy( Path_mask, "*.*" );
 else
  {
   strcpy( Path_mask, Path );

   if( strcmp( Path, "*.*" ) != EQUALLY ) if( Path[ strlen( Path ) - 1 ] != '\\' )
    strcat( Path_mask, "\\*.*" );
  }

 // ��室�� ���� 䠩�.
 HDIR Directory = HDIR_CREATE;
 ULONG All_files = FILE_READONLY | FILE_HIDDEN | FILE_SYSTEM | FILE_ARCHIVED;
 FILEFINDBUF3 Find_buffer = { 0 };
 ULONG Buffer_length = sizeof( FILEFINDBUF3 );
 ULONG Find_count = 1;

 APIRET Report = DosFindFirst( Path_mask, &Directory, All_files, &Find_buffer, Buffer_length, &Find_count, FIL_STANDARD );

 // �᫨ 䠩��� ��� - ���室�� � ��ॡ��� ��⠫����.
 BYTE Perform_directory_search = 0;
 if( Report == ERROR_NO_MORE_FILES ) Perform_directory_search = 1;
 // � �᫨ 䠩� ������ - ��ࠡ��뢠�� ���.
 else
  if( Report == NO_ERROR )
   {
    MakePathname( Static_path, Path, Find_buffer.achName );
    ProcessFile( Static_path );
   }
  else
   {
    DosFindClose( Directory );

    delete Path_mask;
    return;
   }

  // ��ॡ�ࠥ� 䠩��.
  if( !Perform_directory_search )
   while( Report == NO_ERROR )
    {
     // ��室�� ᫥���騩 䠩�.
     Find_count = 1;
     Report = DosFindNext( Directory, &Find_buffer, Buffer_length, &Find_count );

     // ��ࠡ��뢠�� ���.
     if( Report == NO_ERROR )
      {
       MakePathname( Static_path, Path, Find_buffer.achName );
       ProcessFile( Static_path );
      }
    }

 // �����蠥� ����.
 DosFindClose( Directory );

 // ��ॡ�� ��⠫����. ��室�� ���� ��⠫��.
 Directory = HDIR_CREATE;
 Find_count = 1;

 Report = DosFindFirst( Path_mask, &Directory, All_files | MUST_HAVE_DIRECTORY, &Find_buffer, Buffer_length, &Find_count, FIL_STANDARD );

 // �᫨ ��⠫�� ������ - ��ࠡ��뢠�� ���.
 if( Report == NO_ERROR )
  {
   MakePathname( Static_path, Path, Find_buffer.achName );
   if( Include_dirs ) if( Path[ 0 ] != NULL ) ProcessFile( Path );
  }
 else
  {
   DosFindClose( Directory );

   delete Path_mask;
   return;
  }

 // ��ॡ�ࠥ� ��⠫���.
 while( Report == NO_ERROR )
  {
   // ��室�� ᫥���騩 ��⠫��.
   Find_count = 1;
   Report = DosFindNext( Directory, &Find_buffer, Buffer_length, &Find_count );

   // ��ࠡ��뢠�� ���.
   if( Report == NO_ERROR )
    {
     if( strcmp( Find_buffer.achName, "." ) == EQUALLY ) continue;
     if( strcmp( Find_buffer.achName, ".." ) == EQUALLY ) continue;
     MakePathname( Static_path, Path, Find_buffer.achName );
    }
   else break;

   // �⮡� ��ࠡ���� 䠩�� � �⮬ ��⠫���, ��뢠�� FindFiles() ����୮.
   CHAR* New_path = new CHAR[ strlen( Static_path ) + SIZE_OF_NAME ];

   if( New_path != NULL )
    {
     strcpy( New_path, Static_path );
     FindFiles( New_path, Include_dirs );
     delete New_path;
    }
  }

 // �����蠥� ����.
 DosFindClose( Directory );

 delete Path_mask;

 // ������.
 return;
}

// ��� ���⠢��� ������ ��� 䠩��. ���

VOID MakePathname( PCHAR Static_path, PCHAR Path, PCHAR Name )
{
 // ���⠢�塞 ���.
 if( Path[ 0 ] == NULL ) strcpy( Static_path, Name );
 else
  {
   strcpy( Static_path, Path );
   strcat( Static_path, "\\" );
   strcat( Static_path, Name );
  }

 // ������.
 return;
}
