#include "stdafx.h"
#include "MZip.h"
#include "zip/zlib.h"
#include <memory.h>
#include <string.h>
#include "MDebug.h"
#include <tchar.h>
#include <io.h>

typedef unsigned long dword;
typedef unsigned short word;

#define MRS_ZIP_CODE	0x05030207
#define MRS2_ZIP_CODE	0x05030208

#pragma pack(2)
struct MZip::MZIPLOCALHEADER{
	enum{
		SIGNATURE   = 0x04034b50,
		SIGNATURE2  = 0x85840000,
		COMP_STORE  = 0,
		COMP_DEFLAT = 8,
	};

	dword   sig;
	word    version;
	word    flag;
	word    compression;      // COMP_xxxx
	word    modTime;
	word    modDate;
	dword   crc32;
	dword   cSize;
	dword   ucSize;
	word    fnameLen;         // Filename string follows header.
	word    xtraLen;          // Extra field follows filename.
};

struct MZip::MZIPDIRHEADER{
	enum{
		SIGNATURE = 0x06054b50,
	};

	dword   sig;
	word    nDisk;
	word    nStartDisk;
	word    nDirEntries;
	word    totalDirEntries;
	dword   dirSize;
	dword   dirOffset;
	word    cmntLen;
};

struct MZip::MZIPDIRFILEHEADER{
	enum{
		SIGNATURE   = 0x02014b50,
		SIGNATURE2  = 0x05024b80,
		COMP_STORE  = 0,
		COMP_DEFLAT = 8,
	};

	dword   sig;
	word    verMade;
	word    verNeeded;
	word    flag;
	word    compression;      // COMP_xxxx
	word    modTime;
	word    modDate;
	dword   crc32;
	dword   cSize;            // Compressed size
	dword   ucSize;           // Uncompressed size
	word    fnameLen;         // Filename string follows header.
	word    xtraLen;          // Extra field follows filename.
	word    cmntLen;          // Comment field follows extra field.
	word    diskStart;
	word    intAttr;
	dword   extAttr;
	dword   hdrOffset;

	char *GetName   () const { return (char *)(this + 1);   }
	char *GetExtra  () const { return GetName() + fnameLen; }
	char *GetComment() const { return GetExtra() + xtraLen; }
};

#pragma pack()

#define KEY1 10		//1-255
#define KEY2 20		//1-255
#define KEY3 56 	//1-255
#define KEY4 111 	//1-255
#define KEY5 56 	//1-255
#define KEY6 88 	//1-255
#define KEY7 254 	//1-255

#define JUNK_CODE \
__asm{push ecx} \
__asm{push edx} \
__asm{push eax} \
__asm{inc eax} \
__asm{mov edx,eax} \
__asm{inc edx} \
__asm{mov ecx, edx} \
__asm{inc ecx} \
__asm{pop eax} \
__asm{pop edx} \
__asm{pop ecx} \
__asm{push eax} \
__asm{mov eax, edx} \
__asm{xor eax, eax} \
__asm{inc eax} \
__asm{pop eax} \
__asm{inc eax} \
__asm{push eax} \
__asm{mov eax, edx} \
__asm{dec edx} \
__asm{mov edx, eax} \
__asm{pop eax} \
__asm{dec eax}


BYTE KEYB[256] =
{
	0x48, 0x65, 0x72, 0x6f, 0x20, 0x42, 0x61, 0x6e, 0x61, 0x6e, 0x61, 0x20, 0x45, 0x6e, 0x63, 0x72,
	0x79, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x41, 0x73, 0x73, 0x65, 0x6d, 0x62, 0x6c, 0x65, 0x20,
	0x41, 0x6c, 0x67, 0x6f, 0x72, 0x69, 0x74, 0x68, 0x6d, 0x48, 0x65, 0x72, 0x6f, 0x20, 0x42, 0x61,
	0x6e, 0x61, 0x6e, 0x61, 0x20, 0x45, 0x6e, 0x63, 0x72, 0x79, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x20,
	0x41, 0x73, 0x73, 0x65, 0x6d, 0x62, 0x6c, 0x65, 0x20, 0x41, 0x6c, 0x67, 0x6f, 0x72, 0x69, 0x74,
	0x68, 0x6d, 0x48, 0x65, 0x72, 0x6f, 0x20, 0x42, 0x61, 0x6e, 0x61, 0x6e, 0x61, 0x20, 0x45, 0x6e,
	0x63, 0x72, 0x79, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x41, 0x73, 0x73, 0x65, 0x6d, 0x62, 0x6c,
	0x65, 0x20, 0x41, 0x6c, 0x67, 0x6f, 0x72, 0x69, 0x74, 0x68, 0x6d, 0x48, 0x65, 0x72, 0x6f, 0x20,
	0x42, 0x61, 0x6e, 0x61, 0x6e, 0x61, 0x20, 0x45, 0x6e, 0x63, 0x72, 0x79, 0x70, 0x74, 0x69, 0x6f,
	0x6e, 0x20, 0x41, 0x73, 0x73, 0x65, 0x6d, 0x62, 0x6c, 0x65, 0x20, 0x41, 0x6c, 0x67, 0x6f, 0x72,
	0x69, 0x74, 0x68, 0x6d, 0x48, 0x65, 0x72, 0x6f, 0x20, 0x42, 0x61, 0x6e, 0x61, 0x6e, 0x61, 0x20,
	0x45, 0x6e, 0x63, 0x72, 0x79, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x41, 0x73, 0x73, 0x65, 0x6d,
	0x62, 0x6c, 0x65, 0x20, 0x41, 0x6c, 0x67, 0x6f, 0x72, 0x69, 0x74, 0x68, 0x6d, 0x48, 0x65, 0x72,
	0x6f, 0x20, 0x42, 0x61, 0x6e, 0x61, 0x6e, 0x61, 0x20, 0x45, 0x6e, 0x63, 0x72, 0x79, 0x70, 0x74,
	0x69, 0x6f, 0x6e, 0x20, 0x41, 0x73, 0x73, 0x65, 0x6d, 0x62, 0x6c, 0x65, 0x20, 0x41, 0x6c, 0x67,
	0x6f, 0x72, 0x69, 0x74, 0x68, 0x6d, 0x48, 0x65, 0x72, 0x6f, 0x20, 0x42, 0x61, 0x6e, 0x61, 0x6e
};

char KEYS[10] =
{
	'H', 'E', 'R', 'O', 'B', 'A', 'N', 'A', 'N', 'A'
};

void RecoveryChar(char* pData,int _size)
{
	if(!pData) return;

	for(int i=0;i<_size;i++) 
	{
		unsigned char b = pData[i];
		for (int d = 0; d < 10; d++)
		{
			b = b ^ KEYS[d % (sizeof(KEYS) / sizeof(char))];
		}
		pData[i] = b;
	}
}

MZip::MZip(void)
{
	m_fp = NULL;
	m_pDirData = NULL;
	m_ppDir = NULL;
	m_nDirEntries = 0;
	m_nZipMode = ZMode_Zip;
//	m_dwReadMode = MZIPREADFLAG_ZIP | MZIPREADFLAG_MRS | MZIPREADFLAG_MRS2 | MZIPREADFLAG_FILE;
	m_dwReadMode = 0;
}

MZip::~MZip(void)
{
	Finalize();
}

bool MZip::isReadAble(unsigned long mode)
{
	if(m_nZipMode == ZMode_Zip) {
		return ( MZIPREADFLAG_ZIP & mode) ? true : false ; 	
	}
	else if(m_nZipMode == ZMode_Mrs) {
		return ( MZIPREADFLAG_MRS & mode) ? true : false ; 
	}
	else if(m_nZipMode == ZMode_Mrs2) {
		return ( MZIPREADFLAG_MRS2 & mode) ? true : false ; 
	}
	return false;
}

bool MZip::Initialize(FILE* fp,unsigned long ReadMode)
{
	if(fp==NULL) return false;

	// zip , mrs1 ,mrs2 를 읽어야한다..
	// publish 면,,, zip 을 읽지 못하도록 옵션에 모드를 둔다.. 
	// 파일에서는 읽기 불가 + zip 가능여부 + mrs1 가능여부..
	// zip 을 읽을 필요가 있는가?

	m_dwReadMode = ReadMode;

	if(isZip(fp)) {
		m_nZipMode = ZMode_Zip;
		// 플레그에서 지원하지 않으면...
		if(isMode(MZIPREADFLAG_ZIP)==false)
			return false;
	}
	else if(isVersion1Mrs(fp)) {
		m_nZipMode = ZMode_Mrs;
		if(isMode(MZIPREADFLAG_MRS)==false)
			return false;
	}
	else {//mrs2 이상...
		m_nZipMode = ZMode_Mrs2;
		if(isMode(MZIPREADFLAG_MRS2)==false)
			return false;
	}
	
	MZIPDIRHEADER dh;

	fseek(fp, -(int)sizeof(dh), SEEK_END);
	long dhOffset = ftell(fp);
	memset(&dh, 0, sizeof(dh));
	fread(&dh, sizeof(dh), 1, fp);

	if( m_nZipMode>=ZMode_Mrs2 )							// mrs2 이상부터 데이터 복구..
		RecoveryChar((char*)&dh,sizeof(MZIPDIRHEADER));		// v2 이상이면..

	// 데이터를 조작하는 경우.... zip , mrs1 , mrs2 모두아닐경우...

	if( dh.sig != MRS2_ZIP_CODE && dh.sig != MRS_ZIP_CODE && dh.sig != MZIPDIRHEADER::SIGNATURE ) {
		return false;		
	}

	fseek(fp, dhOffset - dh.dirSize, SEEK_SET);

	m_pDirData = new char[dh.dirSize + dh.nDirEntries*sizeof(*m_ppDir)];
	memset(m_pDirData, 0, dh.dirSize + dh.nDirEntries*sizeof(*m_ppDir));
	fread(m_pDirData, dh.dirSize, 1, fp);

	if( m_nZipMode>=ZMode_Mrs2 )
		RecoveryChar( (char*)m_pDirData , dh.dirSize );//mrs 라면 변환..

	char *pfh = m_pDirData;
	m_ppDir = (const MZIPDIRFILEHEADER **)(m_pDirData + dh.dirSize);

	for (int i = 0; i < dh.nDirEntries; i++){
		MZIPDIRFILEHEADER& fh = *(MZIPDIRFILEHEADER*)pfh;

		m_ppDir[i] = &fh;

		if(fh.sig != MZIPDIRFILEHEADER::SIGNATURE){
			if(fh.sig != MZIPDIRFILEHEADER::SIGNATURE2) {
				delete[] m_pDirData;
				m_pDirData = NULL;
				return false;
			}
		}

		{
			pfh += sizeof(fh);

			for (int j = 0; j < fh.fnameLen; j++)
			if (pfh[j] == '/')
			  pfh[j] = '\\';

			pfh += fh.fnameLen + fh.xtraLen + fh.cmntLen;
		}
	}

	m_nDirEntries = dh.nDirEntries;
	m_fp = fp;

	return true;
}

bool MZip::Finalize()
{
	if(m_pDirData!=NULL) {
		delete[] m_pDirData;
		m_pDirData=NULL;
	}

	m_fp = NULL;
	m_ppDir = NULL;
	m_nDirEntries = 0;

	return true;
}

int MZip::GetFileCount(void) const
{
	return m_nDirEntries;
}

void MZip::GetFileName(int i, char *szDest) const
{
	if(szDest!=NULL){
		if (i < 0 || i >= m_nDirEntries){
			*szDest = '\0';
		}
		else{
			memcpy(szDest, m_ppDir[i]->GetName(), m_ppDir[i]->fnameLen);
			szDest[m_ppDir[i]->fnameLen] = '\0';
		}
	}
}

int t_strcmp(const char* str1,const char* str2)
{
	int len = strlen(str1);
	if(strlen(str2)!=len) return -1;
	
	for(int i=0;i<len;i++) {

		if(str1[i] != str2[i]) {
			if(	((str1[i]=='\\') || (str1[i]=='/')) && ((str1[i]=='\\') || (str1[i]=='/')) ) {
				continue;
			}
			else
				return -1;
		}
	}
	return 0;
}

int MZip::GetFileIndex(const char* szFileName) const
{
	if(szFileName==NULL) return -1;

	char szSourceName[256];
	for(int i=0; i<GetFileCount();i++){
		GetFileName(i, szSourceName);
//		if(strcmp(szFileName, szSourceName)==0) 
		if(t_strcmp(szFileName, szSourceName)==0) 
			return i;
	}

	return -1;
}

int MZip::GetFileLength(int i) const
{
	if(i<0 || i>=m_nDirEntries)
		return 0;
	else
		return m_ppDir[i]->ucSize;
}

int MZip::GetFileLength(const char* filename)
{
	int index = GetFileIndex(filename);

	if(index == -1) return 0;

	return GetFileLength(index);
}

unsigned int MZip::GetFileCRC32(int i)
{
	if(i<0 || i>=m_nDirEntries)
		return 0;
	else
		return m_ppDir[i]->crc32;
}

unsigned int MZip::GetFileCRC32(const char* filename)
{
	int index = GetFileIndex(filename);

	if(index == -1) return 0;

	return GetFileCRC32(index);
}

unsigned int MZip::GetFileTime(int i)
{
	if(i<0 || i>=m_nDirEntries)
		return 0;
	else
		return MAKELONG(m_ppDir[i]->modTime,m_ppDir[i]->modDate);
}

unsigned int MZip::GetFileTime(const char* filename)
{
	int index = GetFileIndex(filename);

	if(index == -1) return 0;

	return GetFileCRC32(index);
}


bool MZip::ReadFile(int i, void* pBuffer, int nMaxSize)
{
	if (pBuffer==NULL || i<0 || i>=m_nDirEntries)
		return false;

	fseek(m_fp, m_ppDir[i]->hdrOffset, SEEK_SET);
	MZIPLOCALHEADER h;

	fread(&h, sizeof(h), 1, m_fp);

	if (m_nZipMode >= ZMode_Mrs2) {
		RecoveryChar((char*)&h, sizeof(h));
	}

	if (h.sig != MZIPLOCALHEADER::SIGNATURE) {
		if (h.sig != MZIPLOCALHEADER::SIGNATURE2) {
			return false;
		}
	}

	fseek(m_fp, h.fnameLen + h.xtraLen, SEEK_CUR);// MRS 의경우 복구 안해 줬음..

	if(h.compression==MZIPLOCALHEADER::COMP_STORE){

		fread(pBuffer, h.cSize, 1, m_fp);
		return true;
	}
	else if(h.compression!=MZIPLOCALHEADER::COMP_DEFLAT)
		return false;

	char *pData = new char[h.cSize];
	if(pData==NULL) return false;

	memset(pData, 0, h.cSize);

	int pos = ftell(m_fp);

	fread(pData, h.cSize, 1, m_fp);

	z_stream stream;
	int err;

//#define min(_a, _b)	((_a<=_b)?_a:_b)
	stream.zalloc = (alloc_func)0;
	stream.zfree = (free_func)0;
	stream.opaque = (voidpf)0;

	stream.next_in = (Bytef*)pData;
	stream.avail_in = (uInt)h.cSize;
	stream.next_out = (Bytef*)pBuffer;
	stream.avail_out = min((unsigned int)nMaxSize, h.ucSize);

	err = inflateInit2(&stream, -MAX_WBITS);
	if(err == Z_OK){
		err = inflate(&stream, Z_FINISH);
		inflateEnd(&stream);
		if (err==Z_STREAM_END) err = Z_OK;
		inflateEnd(&stream);
	}

	delete[] pData;

#define CHECK_ZLIB_CRC32
#ifdef CHECK_ZLIB_CRC32
	uLong crc = crc32(0L, Z_NULL, 0);
	crc = crc32(crc, (const Bytef*)pBuffer, stream.total_out);

	if(h.crc32 != crc)
	{
		mlog("crc error \n");
		return false;
	}
#endif

	if(err!=Z_OK) 
		return false;

	return true;
}

bool MZip::ReadFile(const char* filename, void* pBuffer, int nMaxSize)
{
	int index = GetFileIndex(filename);

	if(index == -1) return false;

	return ReadFile(index , pBuffer , nMaxSize);
}

static char _fileheaderReader[1024*16];// sizeof(fh) + fh.fnameLen + fh.xtraLen + fh.cmntLen 의 크기..대충
static int	_fileheaderReaderSize = 0;

bool MZip::isZip(FILE* fp)
{
	fseek(fp, 0, SEEK_SET);
	DWORD sig = 0;
	fread(&sig, 4, 1, fp);

	if(sig == MZIPLOCALHEADER::SIGNATURE)
		return true;

	return false;
}

bool MZip::isVersion1Mrs(FILE* fp)
{
	fseek(fp, 0, SEEK_SET);
	DWORD sig = 0;
	fread(&sig, 4, 1, fp);

	if(sig == MZIPLOCALHEADER::SIGNATURE2)
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////////////

FNode::FNode()
{
	memset(m_name,0,256);
	m_size	 = 0;
	m_offset = 0;
}

void FNode::SetName(char* str)	
{
	if(strlen(str) > 255) return;
	strcpy(m_name,str);
	str[255] = 0;
}

/////////////////////////////////////////////////////////////////////////////////

FFileList::FFileList()
{

}

FFileList::~FFileList() 
{
	DelAll();
}

void FFileList::Add(FNode* pNode) 
{
	push_back(pNode);
}

void FFileList::DelAll() 
{
	if(size()==0) return;

	iterator node;

	for(node = begin(); node != end(); ) 
	{
		delete (*node);
		(*node) = NULL;
		node = erase(node);
	}
}

/////////////////////////////////////////////////////////////////////////////////

// 지정된 경로에서 파일 리스트 얻어내기 

bool GetDirList(char* path,	FFileList& pList)
{
	struct _finddata_t file_t;
	long hFile;

	FNode* pNode;

	if( (hFile = _findfirst( path , &file_t )) != -1L ) {
		do{
			if(strcmp(file_t.name, "." )==0)	continue;
			if(strcmp(file_t.name, "..")==0)	continue;
			if( !(file_t.attrib & _A_SUBDIR) )	continue;

			pNode = new FNode;
			pNode->SetName(file_t.name);
			pList.Add(pNode);

		} 
		while( _findnext( hFile, &file_t ) == 0 );

		_findclose(hFile);
	}

	return true;
}

// 지정된 경로에서 디렉토리 리스트 얻어내기 

bool GetFileList(char* path,FFileList& pList)
{
	struct _finddata_t file_t;
	long hFile;

	FNode* pNode;

	if( (hFile = _findfirst( path , &file_t )) != -1L ) {
		do{
			if(strcmp(file_t.name, "." )==0) continue;
			if(strcmp(file_t.name, "..")==0) continue;
			if(file_t.attrib & _A_SUBDIR )	 continue;

			pNode = new FNode;
			pNode->SetName(file_t.name);
			pList.Add(pNode);

		} 
		while( _findnext( hFile, &file_t ) == 0 );

		_findclose(hFile);
	}

	return true;
}

bool GetFileListWin(char* path,FFileList& pList)
{

	WIN32_FIND_DATA		file_t;
	HANDLE				hFile;

	FNode* pNode;

	if( (hFile = FindFirstFile( path , &file_t )) != INVALID_HANDLE_VALUE ) {

		do {

			if(strcmp(file_t.cFileName, "." )==0)					continue;
			if(strcmp(file_t.cFileName, "..")==0)					continue;
			if(file_t.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )	{
				char _path[1024];

				int len = strlen(path);

				strcpy(_path,path);
				_path[len-1] = 0;
				strcat(_path,file_t.cFileName);
				strcat(_path,"/*");

				GetFileListWin(_path, pList);
				continue;
			}

			int len = (int)strlen(path);

			char temp_name[1024];

			if(len > 1) {
				strncpy(temp_name,path,len-1);
				temp_name[len-1]=0;
				strcat(temp_name,file_t.cFileName);
			}
			else {
				strcpy(temp_name,file_t.cFileName);
			}
			
			pNode = new FNode;
			pNode->SetName(temp_name);
			pList.Add(pNode);

		} while( FindNextFile( hFile, &file_t ) );

		FindClose(hFile);
	}

	return true;
}


// 모든 하위폴더의 원하는 파일들 검색..

bool GetFindFileList(char* path,char* ext,FFileList& pList)
{
	struct _finddata_t file_t;
	long hFile;

	FNode* pNode;

	if( (hFile = _findfirst( path , &file_t )) != -1L ) {
		do{
			if(strcmp(file_t.name, "." )==0) continue;
			if(strcmp(file_t.name, "..")==0) continue;

			if(file_t.attrib & _A_SUBDIR ) {
				char _path[256];
				strcpy(_path,file_t.name);
				strcat(_path,"/");
				strcat(_path,path);

				GetFindFileList(_path,ext,pList);
				continue;
			}

			int len = (int)strlen(ext);
			int filelen = (int)strlen(file_t.name);

			char* pName = &file_t.name[filelen-len];

			if(stricmp(pName,ext)==0) {

				int len = (int)strlen(path);

				char temp_name[256];

				if(len > 3) {

					strncpy(temp_name,path,len-3);
					temp_name[len-3]=0;
					strcat(temp_name,file_t.name);
				}
				else {
					strcpy(temp_name,file_t.name);
				}

				pNode = new FNode;
				pNode->SetName(temp_name);
				pList.Add(pNode);

			}

		} 
		while( _findnext( hFile, &file_t ) == 0 );

		_findclose(hFile);
	}

	return true;
}

bool GetFindFileListWin(char* path,char* ext,FFileList& pList)
{

	WIN32_FIND_DATA		file_t;
	HANDLE				hFile;

	FNode* pNode;

	if( (hFile = FindFirstFile( path , &file_t )) != INVALID_HANDLE_VALUE ) {

		do{
			if(strcmp(file_t.cFileName, "." )==0)	continue;
			if(strcmp(file_t.cFileName, "..")==0)	continue;

			if(file_t.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )	{
				char _path[1024];

				int len = strlen(path);

				strcpy(_path,path);
				_path[len-1] = 0;
				strcat(_path,file_t.cFileName);
				strcat(_path,"/*");

				GetFindFileListWin(_path,ext,pList);
				continue;
			}

			int len = (int)strlen(ext);
			int filelen = (int)strlen(file_t.cFileName);

			char* pName = &file_t.cFileName[filelen-len];

			if(stricmp(pName,ext)==0) {

				int len = (int)strlen(path);

				char temp_name[1024];

				if(len > 1) {

					strncpy(temp_name,path,len-1);
					temp_name[len-1]=0;
					strcat(temp_name,file_t.cFileName);
				}
				else {
					strcpy(temp_name,file_t.cFileName);
				}

				pNode = new FNode;
				pNode->SetName(temp_name);
				pList.Add(pNode);

			}

		} while( FindNextFile( hFile, &file_t ) );

		FindClose(hFile);
	}

	return true;
}
