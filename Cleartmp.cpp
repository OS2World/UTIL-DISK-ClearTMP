#define INCL_VIO
#define INCL_DOS
#define INCL_DOSERRORS

#include <OS2.h>
#include <Stdio.h>
#include "FindRec.hpp"

// Страна - Россия.
#define RUSSIAN 866

// Страна, в которой работает приложение.
ULONG Code_page;

// Временный каталог.
PCHAR Temporary_directory;

// ─── Очистка временных каталогов ───

VOID main( INT Parameters_count, CHAR* Parameters[] )
{
 // Узнаем страну, в которой работает приложение.
 ULONG Size_of_list = 0;
 DosQueryCp( sizeof( Code_page ), &Code_page, &Size_of_list );

 // Если параметры не заданы - сообщение об этом.
 if( Parameters_count < 2 )
  {
   // Печатаем сообщение.
   if( Code_page != RUSSIAN )
    {
     printf( "\nClearTMP is a tool for cleaning a temporary directories on a system startup." );
     printf( "\nVersion: 1.1" );
     printf( "\nUsage: ClearTMP <Temporary_directory>\n" );
    }
   else
    {
     printf( "\nПриложение ClearTMP предназначено для очистки временных каталогов при загрузке" );
     printf( "\nOS/2. Использование: ClearTMP <Путь>" );
     printf( "\nВерсия: 1.1\n" );
    }

   // Возврат.
   return;
  }

 // Если длина пути больше 10 букв - возврат. Вдруг наберут "C:"?
 Temporary_directory = Parameters[ 1 ];
 if( strlen( Temporary_directory ) < 10 )
  {
   // Печатаем сообщение.
   if( Code_page != RUSSIAN ) printf( "\nA path length must be longer than 10 letters.\nNote: \"ClearTMP C:\" command is not allowed.\n" );
   else printf( "\nДлина пути должна быть больше 10.\nВызов \"ClearTMP C:\" не разрешен.\n" );

   // Возврат.
   return;
  }

 // Убираем "\" в конце строки.
 INT Length = strlen( Temporary_directory );
 if( Temporary_directory[ Length - 1 ] == '\\' ) Temporary_directory[ Length - 1 ] = NULL;

 // Печатаем сообщение.
 if( Code_page != RUSSIAN ) printf( "Cleaning \"" );
 else printf( "Очистка \"" );
 printf( Temporary_directory ); printf( "\" ...\n" );

 // Очищаем каталог.
 FindFiles( Temporary_directory, 1 );

 // Возврат.
 return;
}

// ─── Обрабатывает файл ───

VOID ProcessFile( PCHAR Name )
{
 // Сам временный каталог трогать не нужно.
 if( strcmp( Name, Temporary_directory ) == EQUALLY ) return;

 // Удаляем файл.
 APIRET Report = DosForceDelete( Name );
 if( Report != NO_ERROR ) DosDeleteDir( Name );

 // Возврат.
 return;
}


