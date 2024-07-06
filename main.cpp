#include <iostream>
#include <curl/curl.h>
#include <string>
#include <fstream>


// Функция обратного вызова для записи заголовков
size_t header_callback(void *ptr, size_t size, size_t nmemb, std::string *data) {
    data->append((char *)ptr, size * nmemb);                                    // Сохраняем информацию
    return size * nmemb;                                                        // Возвращаем количество байт, которые были записаны, чтобы libcurl знал, что все данные обработаны.
}
// Функция обратного вызова для записи тела ответа
size_t write_callback(void *ptr, size_t size, size_t nmemb, std::string *data) {
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

// void *ptr: Указатель на данные, которые передаются в функцию записи данных
// size_t size: Размер одного блока данных
// size_t nmemb: Количество блоков данных
// std::string *data: Указатель на строку, в которую мы будем записывать данные


int main() {
    CURL *curl;
    CURLcode res;                                                               // Переменная для хранения кода возврата функции curl_easy_perform

    std::string branch = "sisyphus";  // Указываем необходимую ветку
    std::string url = "https://rdb.altlinux.org/api/export/branch_binary_packages/" + branch;
    std::string response_headers;                                               // Строки, в которые будет записываться ответ сервера.
    std::string response_body;

    curl_global_init(CURL_GLOBAL_DEFAULT);                                      // Инициализируем библиотеку libcurl
    curl = curl_easy_init();                                                    // Инициализируем сессию libcurl

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());                       // Устанавливаем URL для запроса
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);        // Устанавливаем функцию обратного вызова для записи данных ответа для хеддера
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response_headers);          // Устанавливаем данные, которые будут переданы функции обратного вызова для хеддера
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);          // Тоже самое для тела ответа
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

        // Добавление SSL версии
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);                     // Отключаем проверку подлинности SSL сертификата сервера
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);                     // Отключаем проверку подлинности имени хоста сервера

        res = curl_easy_perform(curl);                                          // Выполняем запрос. Если запрос завершен успешно, функция возвращает CURLE_OK, в противном случае возвращает код ошибки.
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);    // Получаем код ответа сервера.


            std::ofstream headers_file("headers.txt");                          // Открываем файлы для записи
            std::ofstream body_file("body.txt");

            if(headers_file.is_open() && body_file.is_open()){
                headers_file << "Response code: " << response_code << std::endl;// Записываем код ответа
                body_file << "Response code: " << response_code << std::endl;

                headers_file << "Response headers:\n" << response_headers << std::endl; // Записываем заголовки и тело ответа в файлы
                body_file << "Response body:\n" << response_body << std::endl;

                headers_file.close();                                           // Закрываем файлы
                body_file.close();
            } else std::cerr << "Failed to open output files" << std::endl;
            std::cout << "Response code: " << response_code << std::endl;       // Выводим код ответа
//            std::cout << "Response headers:\n" << response_headers << std::endl;      // Выводим хеддер ответа.
//            std::cout << "Response body:\n" << response_body << std::endl;            // Выводим тело ответа.
        }

        curl_easy_cleanup(curl);                                                // Освобождаем ресурсы,
    }

    curl_global_cleanup();                                                      // Завершаем работу с libcurl

    return 0;
}



















