/*********************************************************************/
/*!
*   \file   wifi.c
*
*   \brief  WiFi support.
*
*   \author Paweł Majewski
*
*/
/*********************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_client.h"
#include "esp_log.h"

#include "wifi.h"
#include "wifi_api.h"
#include "leds.h"

/**********************************************************************
Macros
**********************************************************************/

#define URL "http://192.168.0.185:5000/mainview"
#define TAG "wifi"
#define TAG_POST "post"
#define TAG_GET "get"

/**********************************************************************
Local Function
**********************************************************************/
/*********************************************************************/
/*!
 * \brief  WiFi event support.
 *
 * \param  event_handler_arg - event handler.
 * \param  event_base - base event.
 * \param  event_id - event ID.
 * \param  event_data - event data.
 *
 * \return None
 *
 */
/*********************************************************************/
static void wifiEventHandler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        ESP_LOGI(TAG_GET, "WIFI_EVENT_STA_START");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI(TAG_GET, "WIFI_EVENT_STA_CONNECTED");
        turnOnLed(wifiUiStatus);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGE(TAG_GET, "WIFI_EVENT_STA_DISCONNECTED");
        turnOffLed(wifiUiStatus);
        break;
    case IP_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG_GET, "IP_EVENT_STA_GOT_IP");
        break;
    default:
        break;
    }
}

/*********************************************************************/
/*!
 * \brief  Client event GET.
 *
 * \param  evt - HTTP Client events data.
 *
 * \return Error status.
 *
 */
/*********************************************************************/
static esp_err_t clientEventGetHandler(esp_http_client_event_handle_t evt)
{
    esp_err_t err = ESP_OK;
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGE(TAG_GET, "HTTP_EVENT_ERROR");
        err = ESP_FAIL;
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG_GET, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADERS_SENT:
        ESP_LOGI(TAG_GET, "HTTP_EVENT_HEADERS_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGI(TAG_GET, "HTTP_EVENT_ON_HEADER");
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG_GET, "HTTP_EVENT_ON_DATA");
        getData((char *)evt->data);
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG_GET, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGE(TAG_GET, "HTTP_EVENT_DISCONNECTED");
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGI(TAG_GET, "HTTP_EVENT_REDIRECT");
        break;
    default:
        break;
    }

    return err;
}

/*********************************************************************/
/*!
 * \brief  Client event POST.
 *
 * \param  evt - HTTP Client events data.
 *
 * \return Error status.
 *
 */
/*********************************************************************/
static esp_err_t clientEventPostHandler(esp_http_client_event_handle_t evt)
{
    esp_err_t err = ESP_OK;
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGE(TAG_POST, "HTTP_EVENT_ERROR");
        err = ESP_FAIL;
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG_POST, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADERS_SENT:
        ESP_LOGI(TAG_POST, "HTTP_EVENT_HEADERS_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGI(TAG_POST, "HTTP_EVENT_ON_HEADER");
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG_POST, "HTTP_EVENT_ON_DATA");
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG_POST, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGE(TAG_POST, "HTTP_EVENT_DISCONNECTED");
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGI(TAG_POST, "HTTP_EVENT_REDIRECT");
        break;
    default:
        break;
    }

    return err;
}

/**********************************************************************
 Global Function
**********************************************************************/
/*********************************************************************/
/*!
 * \brief  WiFi initialization.
 *
 * \param  None
 *
 * \return None
 *
 */
/*********************************************************************/
void wifiInit(void)
{
    esp_err_t err = ESP_OK;

    err = nvs_flash_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to init nvs flash: %s", esp_err_to_name(err));
    }

    err = esp_netif_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to init netif: %s", esp_err_to_name(err));
    }

    err = esp_event_loop_create_default();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed esp event loop create: %s", esp_err_to_name(err));
    }
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();

    err = esp_wifi_init(&wifi_initiation);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to init wifi: %s", esp_err_to_name(err));
    }

    err = esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifiEventHandler, NULL);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed esp event handler register: %s", esp_err_to_name(err));
    }

    err = esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifiEventHandler, NULL);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed esp event handler register: %s", esp_err_to_name(err));
    }

    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = CONFIG_ESP_WIFI_SSID,
            .password = CONFIG_ESP_WIFI_PASSWORD,
            .failure_retry_cnt = CONFIG_ESP_MAXIMUM_RETRY}};

    err = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed esp wifi set config: %s", esp_err_to_name(err));
    }

    err = esp_wifi_start();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to esp wifi start: %s", esp_err_to_name(err));
    }

    err = esp_wifi_connect();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed esp wifi connect: %s", esp_err_to_name(err));
    }

    //delay for proper wifi initialization
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    ESP_LOGI(TAG, "WIFI was initiated\n");
}

/*********************************************************************/
/*!
 * \brief  GET support.
 *
 * \param  None
 *
 * \return None
 *
 */
/*********************************************************************/
void restGet(void)
{
    esp_err_t err = ESP_FAIL;
    esp_http_client_config_t config_get = {
        .url = URL,
        .method = HTTP_METHOD_GET,
        .cert_pem = NULL,
        .event_handler = clientEventGetHandler};

    esp_http_client_handle_t client = esp_http_client_init(&config_get);
    if (client == NULL) {
        ESP_LOGE(TAG, "Failed to initialize HTTP client");
        return;
    }

    err = esp_http_client_perform(client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

/*********************************************************************/
/*!
 * \brief  POST support.
 *
 * \param  pData - Pointer where the result is stored.
 *
 * \return None
 *
 */
/*********************************************************************/
void restPost(sensorData* pData)
{
    esp_err_t err = ESP_FAIL;
    esp_http_client_config_t config_post = {
        .url = URL,
        .method = HTTP_METHOD_POST,
        .cert_pem = NULL,
        .event_handler = clientEventPostHandler};

    esp_http_client_handle_t client = esp_http_client_init(&config_post);
    if (client == NULL) {
        ESP_LOGE(TAG, "Failed to initialize HTTP client");
        return;
    }

    char* json_data = postData(pData);

    err = esp_http_client_set_post_field(client, json_data, strlen(json_data));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP set post failed: %s", esp_err_to_name(err));
    }

    err = esp_http_client_set_header(client, "Content-Type", "application/json");
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP set header failed: %s", esp_err_to_name(err));
    }

    err = esp_http_client_perform(client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}