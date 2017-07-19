#include "lib/blizzard_nvs.h"

static const char *TAG = "BLIZZARD NVS";

nvs_handle config_nvs_handle;

uint8_t buf8[NAME_MAX_LENGTH];
uint16_t buf16;

void init_blizzard_nvs()
{
  esp_err_t ret_val;

  ret_val = nvs_flash_init();
  if(ret_val != ESP_OK)
    ESP_LOGI(TAG, "NVS INIT FAILED %d", ret_val);

  ret_val = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &config_nvs_handle);
  if(ret_val != ESP_OK)
    ESP_LOGI(TAG, "NVS OPEN FAILED %d", ret_val);
}


void init_nvs_key_pair_default(uint8_t index)
{
  esp_err_t ret_val;
  switch(index){
    case NVS_DEVICE_NAME_INDEX:
      ret_val = nvs_set_str(config_nvs_handle, NVS_DEVICE_NAME_KEY, DEFAULT_DEVICE_NAME);
      if(ret_val != ESP_OK)
        ESP_LOGI(TAG, "ERROR NVS SET DEFAULT DEVICE NAME %d", ret_val);
    break;
    case NVS_SSID_INDEX:
      ret_val = nvs_set_str(config_nvs_handle, NVS_SSID_KEY, DEFAULT_SSID);
      if(ret_val != ESP_OK)
        ESP_LOGI(TAG, "ERROR NVS SET DEFAULT SSID %d", ret_val);
    break;
    case NVS_PASS_INDEX:
      ret_val = nvs_set_str(config_nvs_handle, NVS_PASS_KEY, DEFAULT_PASS);
      if(ret_val != ESP_OK)
        ESP_LOGI(TAG, "ERROR NVS SET DEFAULT PASS %d", ret_val);
    break;
    case NVS_INPUT_MODE_INDEX:
      ret_val = nvs_set_u8(config_nvs_handle, NVS_INPUT_MODE_KEY, DEFAULT_INPUT_MODE);
      if(ret_val != ESP_OK)
        ESP_LOGI(TAG, "ERROR NVS SET DEFAULT INPUT MODE %d", ret_val);
    break;
    case NVS_OUTPUT_MODE_INDEX:
      ret_val = nvs_set_u8(config_nvs_handle, NVS_OUTPUT_MODE_KEY, DEFAULT_OUTPUT_MODE);
      if(ret_val != ESP_OK)
        ESP_LOGI(TAG, "ERROR NVS SET DEFAULT OUTPUT MODE %d", ret_val);
    break;
    case NVS_OWN_IP_ADDRESS_INDEX:
      buf8[3] = 192; buf8[2] = 168; buf8[1] = 1; buf8[0] = 1;
      ret_val = nvs_set_blob(config_nvs_handle, NVS_OWN_IP_ADDRESS_KEY, buf8, sizeof(uint8_t) * 4);
      if(ret_val != ESP_OK)
        ESP_LOGI(TAG, "ERROR NVS SET DEFAULT IP %d", ret_val);
    break;
    case NVS_OWN_ADDRESS_INDEX:
      ret_val = nvs_set_u16(config_nvs_handle, NVS_OWN_ADDRESS_KEY, (uint16_t) DEFAULT_OWN_ADDRESS);
      if(ret_val != ESP_OK)
        ESP_LOGI(TAG, "ERROR NVS SET DEFAULT OWN ADDRESS %d", ret_val);
    break;
    case NVS_OWN_UNIVERSE_INDEX:
      ret_val = nvs_set_u16(config_nvs_handle, NVS_OWN_UNIVERSE_KEY, (uint16_t) DEFAULT_OWN_UNIVERSE);
      if(ret_val != ESP_OK)
        ESP_LOGI(TAG, "ERROR NVS SET DEFAULT OWN UNIVERSE %d", ret_val);
    break;
    case NVS_SLOTS_INDEX:
      ret_val = nvs_set_u16(config_nvs_handle, NVS_SLOTS_KEY, (uint16_t) DEFAULT_SLOTS);
      if(ret_val != ESP_OK)
        ESP_LOGI(TAG, "ERROR NVS SET DEFAULT SLOTS %d", ret_val);
    break;

  }
  ret_val = nvs_commit(config_nvs_handle);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "NVS COMMIT FAIL - init default %d", ret_val);
  }else
    ESP_LOGI(TAG, "COMMITED");
}

void populate_all_dmx_nvs_values()
{
  esp_err_t ret_val;
  size_t length;
  uint8_t buf_ip[4];

  //set dev name
SET_DEV_NAME:
  ret_val = nvs_get_str(config_nvs_handle, NVS_DEVICE_NAME_KEY, NULL, &length);
  ret_val = nvs_get_str(config_nvs_handle, NVS_DEVICE_NAME_KEY, (char*) buf8, &length);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "ERROR NVS GET DEVICE NAME %d", ret_val);
  }
  if(ret_val == ESP_ERR_NVS_NOT_FOUND){
    init_nvs_key_pair_default(NVS_DEVICE_NAME_INDEX);
    goto SET_DEV_NAME;
  }else
    setName((char *) buf8, length);

SET_SSID:
  ret_val = nvs_get_str(config_nvs_handle, NVS_SSID_KEY, NULL, &length);
  ret_val = nvs_get_str(config_nvs_handle, NVS_SSID_KEY, (char*) buf8, &length);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "ERROR NVS GET SSID %d", ret_val);
  }
  if(ret_val == ESP_ERR_NVS_NOT_FOUND){
    init_nvs_key_pair_default(NVS_SSID_INDEX);
    goto SET_SSID;
  }else
    setSSID((char *) buf8, length);

SET_PASS:
  ret_val = nvs_get_str(config_nvs_handle, NVS_PASS_KEY, NULL, &length);
  ret_val = nvs_get_str(config_nvs_handle, NVS_PASS_KEY, (char*) buf8, &length);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "ERROR NVS GET PASS %d", ret_val);
  }
  if(ret_val == ESP_ERR_NVS_NOT_FOUND){
    init_nvs_key_pair_default(NVS_PASS_INDEX);
    goto SET_PASS;
  }else
    setPASS((char *) buf8, length);

SET_INPUT_MODE:
  ret_val = nvs_get_u8(config_nvs_handle, NVS_INPUT_MODE_KEY, buf8);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "ERROR NVS GET INPUT MODE %d", ret_val);
  }
  if(ret_val == ESP_ERR_NVS_NOT_FOUND){
    init_nvs_key_pair_default(NVS_INPUT_MODE_INDEX);
    goto SET_INPUT_MODE;
  }else
    setInputMode(buf8[0]);

SET_OUTPUT_MODE:
  ret_val = nvs_get_u8(config_nvs_handle, NVS_OUTPUT_MODE_KEY, buf8);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "ERROR NVS GET OUTPUT MODE %d", ret_val);
  }
  if(ret_val == ESP_ERR_NVS_NOT_FOUND){
    init_nvs_key_pair_default(NVS_OUTPUT_MODE_INDEX);
    goto SET_OUTPUT_MODE;
  }else
    setOutputMode(buf8[0]);

SET_OWN_IP:
  ret_val = nvs_get_blob(config_nvs_handle, NVS_OWN_IP_ADDRESS_KEY, NULL, &length);
  ret_val = nvs_get_blob(config_nvs_handle, NVS_OWN_IP_ADDRESS_KEY, buf8, &length);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "ERROR NVS GET IP %d", ret_val);
  }
  if(ret_val == ESP_ERR_NVS_NOT_FOUND){
    init_nvs_key_pair_default(NVS_OWN_IP_ADDRESS_INDEX);
    goto SET_OWN_IP;
  }else
    setOwnIPAddress(buf8);

SET_OWN_ADDRESS:
  ret_val = nvs_get_u16(config_nvs_handle, NVS_OWN_ADDRESS_KEY, &buf16);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "ERROR NVS GET OWN ADDRESS %d", ret_val);
  }
  if(ret_val == ESP_ERR_NVS_NOT_FOUND){
    init_nvs_key_pair_default(NVS_OWN_ADDRESS_INDEX);
    goto SET_OWN_ADDRESS;
  }else
    setOwnAddress(buf16);

SET_OWN_UNIVERSE:
  ret_val = nvs_get_u16(config_nvs_handle, NVS_OWN_UNIVERSE_KEY, &buf16);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "ERROR NVS GET OWN UNIVERSE %d", ret_val);
  }
  if(ret_val == ESP_ERR_NVS_NOT_FOUND){
    init_nvs_key_pair_default(NVS_OWN_UNIVERSE_INDEX);
    goto SET_OWN_UNIVERSE;
  }else
    setOwnUniverse(buf16);

SET_SLOTS:
  ret_val = nvs_get_u16(config_nvs_handle, NVS_SLOTS_KEY, &buf16);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "ERROR NVS GET SLOTS %d", ret_val);
  }
  if(ret_val == ESP_ERR_NVS_NOT_FOUND){
    init_nvs_key_pair_default(NVS_SLOTS_INDEX);
    goto SET_SLOTS;
  }else
  {
    if(buf16 == DMX_MAX_SLOTS){buf16--;}
      setSlots(buf16);
  }

}

void update_u8_nvs_val(const char* key, uint8_t value)
{
  esp_err_t ret_val;

  ret_val = nvs_set_u8(config_nvs_handle, key, value);
  if(ret_val != ESP_OK)
    ESP_LOGI(TAG, "ERROR NVS SET u8 %d", ret_val);

  ret_val = nvs_commit(config_nvs_handle);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "NVS COMMIT FAIL - SET u8 %d", ret_val);
  }else
    ESP_LOGI(TAG, "COMMITED U8 %d", value);
}

void update_u16_nvs_val(const char* key, uint8_t value)
{
  esp_err_t ret_val;

  ret_val = nvs_set_u8(config_nvs_handle, key, value);
  if(ret_val != ESP_OK)
    ESP_LOGI(TAG, "ERROR NVS SET u16 %d", ret_val);

  ret_val = nvs_commit(config_nvs_handle);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "NVS COMMIT FAIL - SET u16 %d", ret_val);
  }else
    ESP_LOGI(TAG, "COMMITED 16 %d", value);
}

void update_blob_nvs_val(const char* key, uint8_t* value, uint8_t length)
{
  esp_err_t ret_val;

  ret_val = nvs_set_blob(config_nvs_handle, key, value, sizeof(uint8_t) * length);
  if(ret_val != ESP_OK)
    ESP_LOGI(TAG, "ERROR NVS SET BLOB %d", ret_val);

  ret_val = nvs_commit(config_nvs_handle);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "NVS COMMIT FAIL - SET BLOB %d", ret_val);
  }else
    ESP_LOGI(TAG, "COMMITED BLOB");
}

void update_str_nvs_val(const char* key, char* value)
{
  esp_err_t ret_val;
  size_t length;

  ret_val = nvs_set_str(config_nvs_handle, key, (const char*) value);
  if(ret_val != ESP_OK)
    ESP_LOGI(TAG, "ERROR NVS SET STR %d", ret_val);

  ret_val = nvs_commit(config_nvs_handle);
  if(ret_val != ESP_OK){
    ESP_LOGI(TAG, "NVS COMMIT FAIL - SET STR %d", ret_val);
  }else
    ESP_LOGI(TAG, "COMMITED STR");
}

void print_nvs_values(uint8_t index)
{
  esp_err_t ret_val;
  size_t length;
  switch(index)
  {
    case NVS_DEVICE_NAME_INDEX:
      ret_val = nvs_get_str(config_nvs_handle, NVS_DEVICE_NAME_KEY, NULL, &length);
      ret_val = nvs_get_str(config_nvs_handle, NVS_DEVICE_NAME_KEY, (char*) buf8, &length);
      if(ret_val != ESP_OK){
        ESP_LOGI(TAG, "ERROR NVS GET DEVICE NAME %d", ret_val);
      }else
        ESP_LOGI(TAG, "DEVICE NAME: %s", buf8);
    break;
    case NVS_SSID_INDEX:
      ret_val = nvs_get_str(config_nvs_handle, NVS_SSID_KEY, NULL, &length);
      ret_val = nvs_get_str(config_nvs_handle, NVS_SSID_KEY, (char*) buf8, &length);
      if(ret_val != ESP_OK){
        ESP_LOGI(TAG, "ERROR NVS GET SSID %d", ret_val);
      }else
        ESP_LOGI(TAG, "SSID: %s", buf8);
    break;
    case NVS_PASS_INDEX:
      ret_val = nvs_get_str(config_nvs_handle, NVS_PASS_KEY, NULL, &length);
      ret_val = nvs_get_str(config_nvs_handle, NVS_PASS_KEY, (char*) buf8, &length);
      if(ret_val != ESP_OK){
        ESP_LOGI(TAG, "ERROR NVS GET PASS %d", ret_val);
      }else
        ESP_LOGI(TAG, "PASS: %s", buf8);
    break;
    case NVS_INPUT_MODE_INDEX:
      ret_val = nvs_get_u8(config_nvs_handle, NVS_INPUT_MODE_KEY, buf8);
      if(ret_val != ESP_OK){
        ESP_LOGI(TAG, "ERROR NVS GET INPUT MODE %d", ret_val);
      }else
        ESP_LOGI(TAG, "INPUT MODE: %d", buf8[0]);
    break;
    case NVS_OUTPUT_MODE_INDEX:
      ret_val = nvs_get_u8(config_nvs_handle, NVS_OUTPUT_MODE_KEY, buf8);
      if(ret_val != ESP_OK){
        ESP_LOGI(TAG, "ERROR NVS GET OUTPUT MODE %d", ret_val);
      }else
        ESP_LOGI(TAG, "OUTPUT MODE: %d", buf8[0]);
    break;
    case NVS_OWN_IP_ADDRESS_INDEX:
      ret_val = nvs_get_blob(config_nvs_handle, NVS_OWN_IP_ADDRESS_KEY, NULL, &length);
      ret_val = nvs_get_blob(config_nvs_handle, NVS_OWN_IP_ADDRESS_KEY, buf8, &length);
      if(ret_val != ESP_OK){
        ESP_LOGI(TAG, "ERROR NVS GET IP %d", ret_val);
      }else
        ESP_LOGI(TAG, "IP: %d.%d.%d.%d", buf8[3], buf8[2], buf8[1], buf8[0]);
    break;
    case NVS_OWN_ADDRESS_INDEX:
      ret_val = nvs_get_u16(config_nvs_handle, NVS_OWN_ADDRESS_KEY, &buf16);
      if(ret_val != ESP_OK){
        ESP_LOGI(TAG, "ERROR NVS GET OWN ADDRESS %d", ret_val);
      }else
        ESP_LOGI(TAG, "OWN ADDRESS: %d", buf16);
    break;
    case NVS_OWN_UNIVERSE_INDEX:
      ret_val = nvs_get_u16(config_nvs_handle, NVS_OWN_UNIVERSE_KEY, &buf16);
      if(ret_val != ESP_OK){
        ESP_LOGI(TAG, "ERROR NVS GET OWN UNIVERSE %d", ret_val);
      }else
        ESP_LOGI(TAG, "OWN UNIVERSE: %d", buf16);
    break;
    case NVS_SLOTS_INDEX:
      ret_val = nvs_get_u16(config_nvs_handle, NVS_SLOTS_KEY, &buf16);
      if(ret_val != ESP_OK){
        ESP_LOGI(TAG, "ERROR NVS GET SLOTS %d", ret_val);
      }else
        ESP_LOGI(TAG, "SLOTS: %d", buf16);
    break;
  }
}
