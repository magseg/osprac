#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/mman.h>

int createFile(char* name){
    FILE* fd = fopen(name, "w");

    if(fd == NULL){
        /* Выдаем сообщение об ошибке и завершаем работу, если файл не удалось открыть */
        printf("Ошибка при открытии файла!\n");
       return 1;
    }
    
    fputs("Текстик в файлике", fd);

    fclose(fd);

    return 0;
}

// Получаем указатель на название файла в папке nfile или без неё.
char* getFileName(int i, int withTemp){
    char* filename = malloc(sizeof("100"));

    if(withTemp == 0){
        sprintf(filename, "nfile/%03d", i);
    }
    else{
        sprintf(filename, "%03d", i);
    }

    return filename;
}


void mFree(char* f1, char* f2){
    free(f1);
    free(f2);
}

// Папка nfile создаётся через system.

int main(int argc, char* argv[]){
    char* directory_name = "nfile";

    printf("Просмотр директории: %s\n", directory_name);
    
    DIR *dir = opendir(directory_name);

    // Проверка на наличие папки nfile:
    if(dir != NULL){
        // Если папка есть - значит удаляем её и все файлы внутри.
       system("rm -r nfile");
    }

    // Создать папку nfile, её либо не было, либо удалили
    system("mkdir nfile");

    char* firstName = getFileName(0, 0);
    if(createFile(firstName)){
        /* Если файл открыть не удалось, выдаем сообщение об ошибке и завершаем работу */
        free(firstName);
        printf("Ошибка открытия файла!\n");
        exit(1);
    }

    free(firstName);

    int counter = 1;

    while(1){
        char* prevFile = getFileName(counter - 1, 1);
        char* nextFile = getFileName(counter, 0);

        if(symlink(prevFile, nextFile)) {
            mFree(prevFile, nextFile);

            printf("Symlink error\n");
            exit(-1);
        }

        
        // Пытаемся открыть файл и определить, корректная ссылка или нет
        // если битая ссылка, то вылезет Null
        FILE* newFile = fopen(nextFile, "r");
        if(newFile == NULL){
            printf("%d\n", counter - 1);

            mFree(prevFile, nextFile);

            exit(1);
         }

        fclose(newFile);
        mFree(prevFile, nextFile);

        counter++;
    }

    if (closedir(dir) != 0) {
        printf("Ошибка закрытия директории!\n");
        return(-1);
    }

    return 0;
}
