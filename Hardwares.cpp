#include <Windows.h>
#include <Pdh.h>
#include <string>
#include <iostream>
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"pdh.lib")

using namespace std;

class cFuncoes
{
private:

	PDH_HQUERY Dados;
	PDH_HCOUNTER Contador;
	PDH_FMT_COUNTERVALUE Valor;

public:

	void ObterUsoDeRAM()
	{
		MEMORYSTATUSEX Memoria;
		Memoria.dwLength = sizeof(MEMORYSTATUSEX);

		GlobalMemoryStatusEx(&Memoria);

		long DisponivelGB = (double)Memoria.ullAvailPhys / (1024 * 1024 * 1024);
		long DisponivelMB = (double)Memoria.ullAvailPhys / (1024 * 1024);

		long Total = (double)Memoria.ullTotalPhys / (1024 * 1024 * 1024);

		long PaginadaGB = (double)Memoria.ullTotalPageFile / (1024 * 1024 * 1024);
		long PaginadaMB = (double)Memoria.ullTotalPageFile / (1024 * 1024);
		long ConfirmadosGB = (double)Memoria.ullAvailPageFile / (1024 * 1024 * 1024);
		//long ConfirmadosMB = (double)Memoria.ullAvailPageFile / (1024 * 1024);

		long Valor;
		if (DisponivelGB < 1 == true)
		{
			Valor = DisponivelMB;
			cout << "Memória disponível: " << Valor << " MB" << " de: " << Total << " GB Total\n";
		}
		else
		{
			Valor = DisponivelGB;
			cout << "Memória disponível: " << Valor << " GB" << " de: " << Total << " GB Total\n";
		}

		long Paginadas;
		if (PaginadaGB < 1 == true)
		{
			Paginadas = PaginadaMB;
			cout << "Memória paginável: " << Paginadas << " MB de: " << ConfirmadosGB << " GB confirmados em uso\n";
		}
		else
		{
			Paginadas = PaginadaGB;
			cout << "Memória paginável: " << Paginadas << " GB de: " << ConfirmadosGB << " GB confirmados em uso\n";
		}

		cout << "Percentual usado: " << Memoria.dwMemoryLoad << " %\n";
	}

	void ObterUsoDeCPU()
	{
		PdhOpenQuery(0, 0, &Dados);
		
		PDH_STATUS AddEngCounter = PdhAddEnglishCounter(Dados, L"\\Processor Information(_Total)\\% Processor Utility", 0, &Contador);
		if (AddEngCounter == ERROR_SUCCESS)
		{
			PdhCollectQueryData(Dados);
			PdhCollectQueryData(Dados);

			PdhGetFormattedCounterValue(Contador, PDH_FMT_LONG, 0, &Valor);

			Sleep(300);

			if (Valor.longValue > 100 == false)
			{
				cout << "Percentual de CPU usado: " << Valor.longValue << "%\n";
			}

			return ObterUsoDeCPU();

			//Use PdhCloseQuery(Dados); quando a função de pesquisa não for mais necessário.
		}
		else
		{
			cout << "Ocorreu um erro ao obter informações do contador..\n";
		}
	}

}Funcoes;

int main()
{

	cout << "O assistente está verificando o desempenho de hardwares...\n";

	Funcoes.ObterUsoDeRAM();
	Funcoes.ObterUsoDeCPU();

	system("pause");
}
