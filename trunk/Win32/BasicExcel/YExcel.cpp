#include "BasicExcel.hpp"
#include <locale.h>
#include <windows.h> 
#include<string>
using namespace YExcel;

void wcharTochar(const wchar_t *wchar, char *chr, int length)   
{   
	WideCharToMultiByte( CP_ACP, 0, wchar, -1,   chr, length, NULL, NULL );   
}
std::string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	std::string result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "chs");
#if 0
	BasicExcel e;

	// Load a workbook with one sheet, display its contents and save into another file.
	e.Load("example1.xls");	
	BasicExcelWorksheet* sheet1 = e.GetWorksheet("Sheet1");
	if (sheet1)
	{
		size_t maxRows = sheet1->GetTotalRows();
		size_t maxCols = sheet1->GetTotalCols();
		cout << "Dimension of " << sheet1->GetAnsiSheetName() << " (" << maxRows << ", " << maxCols << ")" << endl;

		printf("          ");
		for (size_t c=0; c<maxCols; ++c) printf("%10d", c+1);
		cout << endl;

		for (size_t r=0; r<maxRows; ++r)
		{
			printf("%10d", r+1);
			for (size_t c=0; c<maxCols; ++c)
			{
				BasicExcelCell* cell = sheet1->Cell(r,c);
				switch (cell->Type())
				{
					case BasicExcelCell::UNDEFINED:
						printf("          ");
						break;

					case BasicExcelCell::INT:
						printf("%10d", cell->GetInteger());
						break;

					case BasicExcelCell::DOUBLE:
						printf("%10.6lf", cell->GetDouble());
						break;

					case BasicExcelCell::STRING:
						printf("%10s", cell->GetString());
						break;

					case BasicExcelCell::WSTRING:
						wprintf(L"%10s", cell->GetWString());
						break;
				}
			}
			cout << endl;
		}
	}
	cout << endl;
	e.SaveAs("example2.xls");

	// Create a new workbook with 2 worksheets and write some contents.
	e.New(2);
	e.RenameWorksheet("Sheet1", "Test1");
	BasicExcelWorksheet* sheet = e.GetWorksheet("Test1");
	BasicExcelCell* cell;
	if (sheet)
	{
		for (size_t c=0; c<4; ++c)
		{
			cell = sheet->Cell(0,c);
			cell->Set((int)c);
		}

		cell = sheet->Cell(1,3);
		cell->SetDouble(3.141592654);

		sheet->Cell(1,4)->SetString("Test str1");
		sheet->Cell(2,0)->SetString("Test str2");
		sheet->Cell(2,5)->SetString("Test str1");

		sheet->Cell(4,0)->SetDouble(1.1);
		sheet->Cell(4,1)->SetDouble(2.2);
		sheet->Cell(4,2)->SetDouble(3.3);
		sheet->Cell(4,3)->SetDouble(4.4);
		sheet->Cell(4,4)->SetDouble(5.5);

		sheet->Cell(4,4)->EraseContents();
	}

	sheet = e.AddWorksheet("Test2", 1);
	sheet = e.GetWorksheet(1); 
	if (sheet)
	{
		sheet->Cell(1,1)->SetDouble(1.1);
		sheet->Cell(2,2)->SetDouble(2.2);
		sheet->Cell(3,3)->SetDouble(3.3);
		sheet->Cell(4,4)->SetDouble(4.4);
		sheet->Cell(70,2)->SetDouble(5.5);
	}
	e.SaveAs("example3.xls");

	// Load the newly created sheet and display its contents
	e.Load("example3.xls");

	size_t maxSheets = e.GetTotalWorkSheets();
	cout << "Total number of worksheets: " << e.GetTotalWorkSheets() << endl;
	for (size_t i=0; i<maxSheets; ++i)
	{
		BasicExcelWorksheet* sheet = e.GetWorksheet(i);
		if (sheet)
		{
			size_t maxRows = sheet->GetTotalRows();
			size_t maxCols = sheet->GetTotalCols();
			cout << "Dimension of " << sheet->GetAnsiSheetName() << " (" << maxRows << ", " << maxCols << ")" << endl;

			if (maxRows>0)
			{
				printf("          ");
				for (size_t c=0; c<maxCols; ++c) printf("%10d", c+1);
				cout << endl;
			}

			for (size_t r=0; r<maxRows; ++r)
			{
				printf("%10d", r+1);
				for (size_t c=0; c<maxCols; ++c)
				{
					cout << setw(10) << *(sheet->Cell(r,c));	// Another way of printing a cell content.				
				}
				cout << endl;
			}
			if (i==0)
			{
				ofstream f("example4.csv");
				sheet->Print(f, ',', '\"');	// Save the first sheet as a CSV file.
				f.close();
			}
		}
		cout << endl;
	}
#else 
	
	BasicExcel e;
	e.Load("Item.xls");
	//TODO不支持中文路径
	//e.Load("C:/Documents and Settings/Micheal\桌面/Item.xls");
	printf("loaded\r\n");

	size_t num=e.GetTotalWorkSheets();
	printf("sheetnum:%d\r\n",num);
	for(size_t i=0;i<num;++i)
	{
		//一个工作薄可建256个表格，每个表格名为256个字符。
		wchar_t buffer[256];
		e.GetSheetName(i,buffer);
		char temp[512];
		wcharTochar(buffer,temp,sizeof(temp));
		printf("%s\r\n",temp);

		BasicExcelWorksheet* sheet = e.GetWorksheet(i);
		size_t colnum=sheet->GetTotalCols();
		size_t rownum=sheet->GetTotalRows();
		printf("%d,%d\r\n",colnum,rownum);
		//2003列是256；行是65536
		//2007列是16384；行是1048576
		BasicExcelCell* cell=sheet->Cell(2,3);
		{
			//单元格内容（文本）的长度32767个字符。单元格中只能显示1024个字符；而编辑栏中可以显示全部32767个字符。
			/*wchar_t buffer[32767];
			if(cell->Get(buffer))
			{
				char temp[65535];
				wcharTochar(buffer,temp,sizeof(temp));
				printf("%s\r\n",temp);
			}
			else
				printf("null\r\n");
			*/
			switch (cell->Type())
			{
			case BasicExcelCell::UNDEFINED:
				printf("\r\n");
				break;

			case BasicExcelCell::INT:
				printf("%d\r\n", cell->GetInteger());
				break;

			case BasicExcelCell::DOUBLE:
				//TODO即使是int型还是被解析成DOUBLE型，所以这里使用%g适配两种类型，以便保持输出与原数据完全等同
				//printf("%g\r\n", cell->GetDouble());
				//cout<<cell->GetDouble()<<endl;
				char buffer[64];
				ZeroMemory(buffer,sizeof(buffer));
				sprintf_s(buffer,64,"%g",cell->GetDouble());
				printf("%s\r\n",buffer);
				break;

			case BasicExcelCell::STRING:
				printf("%s\r\n", cell->GetString());
				break;

			case BasicExcelCell::WSTRING:
				wstring str=cell->GetWString();
				//wprintf(L"%s\r\n", cell->GetWString());
				printf("%s\r\n",ws2s(str).c_str());
				break;
			}
		}
		
	}

#endif
	getchar();
	return 0;
}
