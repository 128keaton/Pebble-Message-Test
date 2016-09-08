#include "pebble.h"

static Window *s_main_window;
static TextLayer *s_text_layer;
// Largest expected inbox and outbox message sizes
const uint32_t inbox_size = 64;
const uint32_t outbox_size = 256;

// Declare the dictionary's iterator
DictionaryIterator *out_iter;

// Prepare the outbox buffer for this message


static void inbox_received_callback(DictionaryIterator *iter, void *context) {
  // Is the location name inside this message?
   Tuple *t = dict_read_first(iter);

  if(t) {
    // This value was stored as JS String, which is stored here as a char string
    char *location_name = t->value->cstring;

    // Use a static buffer to store the string for display
    static char s_buffer[12];
    snprintf(s_buffer, sizeof(s_buffer), "%s", location_name);

    // Display in the TextLayer
    text_layer_set_text(s_text_layer, s_buffer);
  }
}

static void outbox_sent_callback(DictionaryIterator *iter, void *context) {
  text_layer_set_text(s_text_layer, "Sent \n message!");
}
static void outbox_failed_callback(DictionaryIterator *iter,
                                      AppMessageResult reason, void *context) {
  // The message just sent failed to be delivered
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message send failed. Reason: %d", (int)reason);
}

static void sendMessage(){
  AppMessageResult result = app_message_outbox_begin(&out_iter);
if(result == APP_MSG_OK) {
  // A dummy value


  // Add an item to ask for weather data
  dict_write_cstring(out_iter, MESSAGE_KEY_message, "Spaghetti!");

  // Send this message
result = app_message_outbox_send();

// Check the result
if(result != APP_MSG_OK) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
}
  
} else {
  // The outbox cannot be used right now
  APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int)result);
}



}
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  sendMessage();
    text_layer_set_text(s_text_layer, "Sent message");

}
static void click_config_provider(void *context) {
  // Subcribe to button click events here
    ButtonId id = BUTTON_ID_SELECT;  // The Select button

    window_single_click_subscribe(id, select_click_handler);

}



static void init() {
  s_main_window = window_create();
  window_stack_push(s_main_window, true);

  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_bounds(window_layer);


  // Open AppMessage
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_open(inbox_size, outbox_size);
  window_set_click_config_provider(s_main_window, click_config_provider);
  
  s_text_layer = text_layer_create(bounds);
  text_layer_set_text(s_text_layer, PBL_IF_RECT_ELSE("  Hello,\n  World!", "Hello, World!"));

  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
#if PBL_ROUND
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_enable_screen_text_flow_and_paging(s_text_layer, 8);
#endif
}



static void deinit() {
  text_layer_destroy(s_text_layer);
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
