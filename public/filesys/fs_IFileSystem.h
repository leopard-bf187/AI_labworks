/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 14.07.2026
*/


#pragma once


#include "fs_dll_types.h"


namespace krystallic
{
	namespace FileSystem
	{
		struct IFileSystem : IBase
		{
			virtual ~IFileSystem() = default;

            virtual ERRCODE SetRootPath(const char* path) = 0; // установка корневой директории
            virtual ERRCODE AddSource(const char* source, EFileSystemDataSourceType dataSourceType) = 0; // добавления источника данных
            virtual ERRCODE BuildIndex() = 0; // построение файловой системы, вызывается после добавления источников
            virtual ERRCODE AddPathKey(const char* key, const char* path) = 0; // добавление ключа,
            virtual ERRCODE CreatePhysicalFileObject(const char** path, IFileIO** outFile) = 0; // создает файл по пути
            virtual ERRCODE CreateFileObjectInMemory(IFileIO** outFile) = 0; // создает объект файла в памяти, который потом можно сохранить по пути, или в архив
            virtual ERRCODE GetPack(const char* path, IPack** outFile) = 0; // получает объект архива
            virtual ERRCODE LoadFile(const char* path, IFileIO** outFile) = 0; // загружает файл из папки ресурсов
            virtual ERRCODE SaveFile(const char* path, IFileIO* fileObj) = 0; // сохраняет файл на диск
            virtual ERRCODE SaveFileToPack(IPack* pack, IFileIO* fileObj) = 0;

            inline static SGuid GUID()
            {
                return { 0x64337c4d, 0x3c97, 0x41bf, { 0x9d, 0x63, 0x1a, 0xd4, 0xc2, 0xa1, 0x84, 0xdc } };
            }
		};
	}
}