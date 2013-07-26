#define INCL_DOSFILEMGR
#define INCL_DOSERRORS

#include <OS2.h>
#include <Stdlib.h>
#include <String.h>

// Названия данных.
#define CONST   const
#define STATIC  static
#define FLOAT   float
#define EQUALLY 0

// Длина строки для имени файла.
#define SIZE_OF_NAME   255
#define SIZE_OF_PATH  1024

// Выполняет поиск файлов.
VOID FindFiles( PCHAR, BYTE );
// Составляет полное имя файла.
VOID MakePathname( PCHAR, PCHAR, PCHAR );

// Обрабатывает файл. Должна быть перегружена пользователем.
VOID ProcessFile( PCHAR );

// ─── Выполняет поиск файлов ───

// Path - путь, с которого надо начать поиск, Include_dirs - надо ли обрабатывать каталоги.
VOID FindFiles( PCHAR Path, BYTE Include_dirs )
{
 // Текущий путь.
 STATIC CHAR Static_path[ SIZE_OF_PATH ];

 // Задаем имена для поиска.
 CHAR* Path_mask = new CHAR[ strlen( Path ) + SIZE_OF_NAME ];
 if( Path_mask == NULL ) return;

 if( Path[ 0 ] == NULL ) strcpy( Path_mask, "*.*" );
 else
  {
   strcpy( Path_mask, Path );

   if( strcmp( Path, "*.*" ) != EQUALLY ) if( Path[ strlen( Path ) - 1 ] != '\\' )
    strcat( Path_mask, "\\*.*" );
  }

 // Находим первый файл.
 HDIR Directory = HDIR_CREATE;
 ULONG All_files = FILE_READONLY | FILE_HIDDEN | FILE_SYSTEM | FILE_ARCHIVED;
 FILEFINDBUF3 Find_buffer = { 0 };
 ULONG Buffer_length = sizeof( FILEFINDBUF3 );
 ULONG Find_count = 1;

 APIRET Report = DosFindFirst( Path_mask, &Directory, All_files, &Find_buffer, Buffer_length, &Find_count, FIL_STANDARD );

 // Если файлов нет - переходим к перебору каталогов.
 BYTE Perform_directory_search = 0;
 if( Report == ERROR_NO_MORE_FILES ) Perform_directory_search = 1;
 // А если файл найден - обрабатываем его.
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

  // Перебираем файлы.
  if( !Perform_directory_search )
   while( Report == NO_ERROR )
    {
     // Находим следующий файл.
     Find_count = 1;
     Report = DosFindNext( Directory, &Find_buffer, Buffer_length, &Find_count );

     // Обрабатываем его.
     if( Report == NO_ERROR )
      {
       MakePathname( Static_path, Path, Find_buffer.achName );
       ProcessFile( Static_path );
      }
    }

 // Завершаем поиск.
 DosFindClose( Directory );

 // Перебор каталогов. Находим первый каталог.
 Directory = HDIR_CREATE;
 Find_count = 1;

 Report = DosFindFirst( Path_mask, &Directory, All_files | MUST_HAVE_DIRECTORY, &Find_buffer, Buffer_length, &Find_count, FIL_STANDARD );

 // Если каталог найден - обрабатываем его.
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

 // Перебираем каталоги.
 while( Report == NO_ERROR )
  {
   // Находим следующий каталог.
   Find_count = 1;
   Report = DosFindNext( Directory, &Find_buffer, Buffer_length, &Find_count );

   // Обрабатываем его.
   if( Report == NO_ERROR )
    {
     if( strcmp( Find_buffer.achName, "." ) == EQUALLY ) continue;
     if( strcmp( Find_buffer.achName, ".." ) == EQUALLY ) continue;
     MakePathname( Static_path, Path, Find_buffer.achName );
    }
   else break;

   // Чтобы обработать файлы в этом каталоге, вызываем FindFiles() повторно.
   CHAR* New_path = new CHAR[ strlen( Static_path ) + SIZE_OF_NAME ];

   if( New_path != NULL )
    {
     strcpy( New_path, Static_path );
     FindFiles( New_path, Include_dirs );
     delete New_path;
    }
  }

 // Завершаем поиск.
 DosFindClose( Directory );

 delete Path_mask;

 // Возврат.
 return;
}

// ─── Составляет полное имя файла. ───

VOID MakePathname( PCHAR Static_path, PCHAR Path, PCHAR Name )
{
 // Составляем имя.
 if( Path[ 0 ] == NULL ) strcpy( Static_path, Name );
 else
  {
   strcpy( Static_path, Path );
   strcat( Static_path, "\\" );
   strcat( Static_path, Name );
  }

 // Возврат.
 return;
}
