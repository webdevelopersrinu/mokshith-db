#include <iostream>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>
#include "client/client.h"

using namespace mokshith;

class MokshithShell {
public:
    MokshithShell(const std::string& host, int port)
        : client_(host, port) {}
    
    void Run() {
        std::cout << "Mokshith DB Shell v1.0\n";
        std::cout << "Type 'help' for help, 'quit' to exit\n\n";
        
        if (!client_.Connect()) {
            std::cerr << "Failed to connect to server\n";
            return;
        }
        
        while (true) {
            char* input = readline("mokshith> ");
            if (!input) break;
            
            std::string query(input);
            add_history(input);
            free(input);
            
            if (query == "quit" || query == "exit") {
                break;
            } else if (query == "help") {
                ShowHelp();
            } else if (!query.empty()) {
                ExecuteQuery(query);
            }
        }
        
        client_.Disconnect();
    }
    
private:
    Client client_;
    
    void ExecuteQuery(const std::string& query) {
        auto start = std::chrono::high_resolution_clock::now();
        
        try {
            auto result = client_.ExecuteQuery(query);
            DisplayResult(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            
            std::cout << "\nQuery executed in " << duration.count() << " ms\n";
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    
    void DisplayResult(const ResultSet& result) {
        // Display column headers
        for (const auto& col : result.GetColumns()) {
            std::cout << std::setw(15) << col.name << " ";
        }
        std::cout << "\n";
        
        // Display separator
        for (size_t i = 0; i < result.GetColumns().size(); ++i) {
            std::cout << std::string(15, '-') << " ";
        }
        std::cout << "\n";
        
        // Display rows
        for (const auto& row : result.GetRows()) {
            for (const auto& value : row) {
                std::cout << std::setw(15) << value.ToString() << " ";
            }
            std::cout << "\n";
        }
        
        std::cout << "\n" << result.GetRowCount() << " rows in set\n";
    }
    
    void ShowHelp() {
        std::cout << "\nMokshith DB Commands:\n";
        std::cout << "  CREATE TABLE ...  - Create a new table\n";
        std::cout << "  INSERT INTO ...   - Insert data\n";
        std::cout << "  SELECT ...        - Query data\n";
        std::cout << "  UPDATE ...        - Update data\n";
        std::cout << "  DELETE FROM ...   - Delete data\n";
        std::cout << "  BEGIN            - Start transaction\n";
        std::cout << "  COMMIT           - Commit transaction\n";
        std::cout << "  ROLLBACK         - Rollback transaction\n";
        std::cout << "  help             - Show this help\n";
        std::cout << "  quit/exit        - Exit shell\n\n";
    }
};

int main(int argc, char* argv[]) {
    std::string host = "localhost";
    int port = 5432;
    
    if (argc > 1) host = argv[1];
    if (argc > 2) port = std::atoi(argv[2]);
    
    MokshithShell shell(host, port);
    shell.Run();
    
    return 0;
}