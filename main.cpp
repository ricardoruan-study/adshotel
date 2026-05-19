#include <iostream>
#include <winsock2.h> // Obrigatório para evitar erros de compilação no Windows
#include <mysql.h>
#include <string>     
#include <sstream>    // Usado para conversões seguras em C++ antigo

using namespace std;

// Configurações globais de acesso ao banco
const char* HOST = "localhost";
const char* USER = "root";
const char* PASSWORD = "ifsp";
const char* DATABASE = "hotel";

// ==========================================
// MÓDULO 1: CONEXÃO COM O BANCO DE DADOS
// ==========================================
void conectarBanco(MYSQL** conn) {
    *conn = mysql_init(NULL);
    if (!(*conn)) {
        cerr << "Falha na inicialização da biblioteca!" << endl;
        return;
    }

    if (!mysql_real_connect(*conn, HOST, USER, PASSWORD, DATABASE, 3306, NULL, 0)) {
        cerr << "Erro na conexão: " << mysql_error(*conn) << endl;
        mysql_close(*conn);
        *conn = NULL; 
    } else {
        cout << "Conectado ao banco de dados com sucesso!\n" << endl;
    }
}

// ==========================================
// MÓDULO 2: EXIBIÇÃO DO MENU
// ==========================================
void exibirMenu() {
    cout << "========== MENU DE RESERVAS ==========" << endl;
    cout << "1 - Listar todos os quartos" << endl;
    cout << "2 - Listar quartos disponiveis" << endl;
    cout << "3 - Cadastrar Hospede" << endl;
    cout << "4 - Realizar Reserva (Novo)" << endl;
    cout << "5 - Listar Reservas Ativas (Novo)" << endl;
    cout << "6 - Sair" << endl;
    cout << "Escolha uma opcao: ";
}

// ==========================================
// MÓDULO 3: LISTAR TODOS OS QUARTOS (SELECT)
// ==========================================
void listarTodosQuartos(MYSQL* conn) {
    const char* select_query = "SELECT id, descricao, valor_diaria, tipo_quarto FROM quartos ";
    if (mysql_query(conn, select_query)) {
        cerr << "Erro ao buscar dados: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res) {
        MYSQL_ROW row; 
        cout << "\n--- Todos os quartos cadastrados ---" << endl;
        
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << (row[0] ? row[0] : "NULL") 
                 << " | Descricao: " << (row[1] ? row[1] : "NULL") 
                 << " | Valor Diaria: " << (row[2] ? row[2] : "NULL") 
                 << " | Tipo de Quarto: " << (row[3] ? row[3] : "NULL") << endl;
        }
        mysql_free_result(res);
    }
}

// ==========================================
// MÓDULO 4: LISTAR QUARTOS DISPONÍVEIS
// ==========================================
void listarQuartosDisponiveis(MYSQL* conn) {
    const char* select_query = "SELECT id, descricao, valor_diaria, tipo_quarto FROM quartos where id not in (SELECT id_quarto FROM reservas) ";
    if (mysql_query(conn, select_query)) {
        cerr << "Erro ao buscar dados: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res) {
        MYSQL_ROW row;
        cout << "\n--- Todos disponiveis para reserva ---" << endl;
        
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << (row[0] ? row[0] : "NULL") 
                 << " | Descricao: " << (row[1] ? row[1] : "NULL") 
                 << " | Valor Diaria: " << (row[2] ? row[2] : "NULL") 
                 << " | Tipo de Quarto: " << (row[3] ? row[3] : "NULL") << endl;
        }
        mysql_free_result(res);
    }
}

// ==========================================
// MÓDULO 5: CADASTRAR HÓSPEDE 
// ==========================================
void cadastrarHospede(MYSQL* conn) {
    string nome, data_nasc;
    int cpf = 0;

    string* ptr_nome = &nome;
    string* ptr_data_nasc = &data_nasc;
    int* ptr_cpf = &cpf;

    cout << "\n--- CADASTRO DE NOVO HÓSPEDE ---" << endl;
    cin.ignore(); 

    cout << "Digite o nome completo: ";
    getline(cin, *ptr_nome);

    cout << "Digite a data de nascimento (AAAA-MM-DD): ";
    getline(cin, *ptr_data_nasc);

    cout << "Digite o CPF (apenas numeros): ";
    cin >> *ptr_cpf;

    stringstream ss;
    ss << *ptr_cpf;
    string cpf_string = ss.str();

    string queryStr = "INSERT INTO hospedes (nome, data_nasc, cpf) VALUES ('" 
                      + *ptr_nome + "', '" 
                      + *ptr_data_nasc + "', " 
                      + cpf_string + ")";
                      
    const char* insert_query = queryStr.c_str();

    if (mysql_query(conn, insert_query)) {
        cerr << "Erro ao cadastrar hospede: " << mysql_error(conn) << endl;
    } else {
        cout << "Hospede '" << *ptr_nome << "' cadastrado com sucesso!" << endl;
    }
}

// ==========================================
// MÓDULO 6: CADASTRAR RESERVA (NOVO)
// Vincula o Quarto ao Hóspede e define o período
// ==========================================
void realizarReserva(MYSQL* conn) {
    int idQuarto = 0, idHospede = 0;
    string dataInicio, dataFim;

    int* ptr_idQuarto = &idQuarto;
    int* ptr_idHospede = &idHospede;
    string* ptr_dataInicio = &dataInicio;
    string* ptr_dataFim = &dataFim;

    cout << "\n--- REALIZAR NOVA RESERVA ---" << endl;
    cout << "Digite o ID do Quarto: ";
    cin >> *ptr_idQuarto;
    cout << "Digite o ID do Hospede: ";
    cin >> *ptr_idHospede;
    
    cin.ignore(); // Limpa o buffer antes de ler strings
    cout << "Digite a data de Check-in (AAAA-MM-DD): ";
    getline(cin, *ptr_dataInicio);
    cout << "Digite a data de Check-out (AAAA-MM-DD): ";
    getline(cin, *ptr_dataFim);

    // Converte os IDs usando stringstream para evitar o erro do compilador
    stringstream ssQuarto, ssHospede;
    ssQuarto << *ptr_idQuarto;
    ssHospede << *ptr_idHospede;

    string queryStr = "INSERT INTO reservas (id_quarto, id_hospede, data_inicio, data_fim) VALUES ("
                      + ssQuarto.str() + ", "
                      + ssHospede.str() + ", '"
                      + *ptr_dataInicio + "', '"
                      + *ptr_dataFim + "')";

    const char* insert_query = queryStr.c_str();

    if (mysql_query(conn, insert_query)) {
        cerr << "Erro ao realizar reserva: " << mysql_error(conn) << endl;
    } else {
        cout << "Reserva vinculada e gravada com sucesso!" << endl;
    }
}

// ==========================================
// MÓDULO 7: LISTAR RESERVAS VÁLIDAS (NOVO)
// Utiliza INNER JOIN para trazer informações legíveis cruzando as tabelas
// ==========================================
void listarReservasValidas(MYSQL* conn) {
    // Busca os dados unindo as 3 tabelas para não mostrar apenas IDs sem sentido na tela
    const char* select_query = "SELECT r.id, h.nome, q.descricao, r.data_inicio, r.data_fim "
                               "FROM reservas r "
                               "INNER JOIN hospedes h ON r.id_hospede = h.id "
                               "INNER JOIN quartos q ON r.id_quarto = q.id";

    if (mysql_query(conn, select_query)) {
        cerr << "Erro ao buscar reservas: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res) {
        MYSQL_ROW row;
        cout << "\n--- RELATÓRIO DE RESERVAS ATIVAS ---" << endl;
        
        while ((row = mysql_fetch_row(res))) {
            cout << "Reserva No: " << (row[0] ? row[0] : "NULL") 
                 << " | Hospede: " << (row[1] ? row[1] : "NULL") 
                 << " | Quarto: " << (row[2] ? row[2] : "NULL") 
                 << " | Entrada: " << (row[3] ? row[3] : "NULL") 
                 << " | Saida: " << (row[4] ? row[4] : "NULL") << endl;
        }
        mysql_free_result(res);
    }
}

// ==========================================
// FLUXO PRINCIPAL DO PROGRAMA
// ==========================================
int main() {
    MYSQL* conn = NULL; 

    conectarBanco(&conn);
    if (!conn) {
        return 1; 
    }

    int opcao = 0;
    int* ptr_opcao = &opcao; 

    // O loop foi ajustado para encerrar na opção 6
    while (*ptr_opcao != 6) {
        exibirMenu();
        cin >> *ptr_opcao; 

        if (*ptr_opcao == 1) {
            listarTodosQuartos(conn);
        } else if (*ptr_opcao == 2) {
            listarQuartosDisponiveis(conn);
        } else if (*ptr_opcao == 3) {
            cadastrarHospede(conn); 
        } else if (*ptr_opcao == 4) {
            realizarReserva(conn); // Novo módulo chamado
        } else if (*ptr_opcao == 5) {
            listarReservasValidas(conn); // Novo módulo chamado
        }

        cout << "\n\n\n";
    }

    mysql_close(conn);
    cout << "Conexao encerrada com sucesso." << endl;
    return 0;
}
