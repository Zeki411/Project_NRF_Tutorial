#include <zephyr/zephyr.h>
#include <logging/log.h>
#include <drivers/gpio.h>
#include <drivers/uart.h>

#define RECEIVE_BUFF_SIZE 10
#define RECEIVE_TIMEOUT 100

LOG_MODULE_REGISTER(UART_LED_DEMO,LOG_LEVEL_DBG);

static uint8_t tx_buf[] = {"nRF Connect SDK Fundamentals Course\n\r"
                            "Press 1-3 on your keyboard to toggle LEDS 1-3 on your development kit\n\r"};
static uint8_t rx_buf[RECEIVE_BUFF_SIZE] = {0};


static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
const struct device *uart= DEVICE_DT_GET(DT_NODELABEL(uart0));

static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
	switch (evt->type) {

	case UART_RX_RDY:
		if((evt->data.rx.len) == 1){

		if(evt->data.rx.buf[evt->data.rx.offset] == '1')
			gpio_pin_toggle_dt(&led0);
		else if (evt->data.rx.buf[evt->data.rx.offset] == '2')
			gpio_pin_toggle_dt(&led1);
		else if (evt->data.rx.buf[evt->data.rx.offset] == '3')
			gpio_pin_toggle_dt(&led2);					
		}
        break;
	case UART_RX_DISABLED:
		uart_rx_enable(dev, rx_buf,sizeof rx_buf,RECEIVE_TIMEOUT); // Enable to continous receiving data with timeout
		break;
		
	default:
		break;
	}
}

void main(void) 
{
    int ret;  

    // To configure UART dynamically, we can use following struct
    const struct uart_config uart_cfg = {
    		.baudrate = 115200,
    		.parity = UART_CFG_PARITY_NONE,
    		.stop_bits = UART_CFG_STOP_BITS_1,
    		.data_bits = UART_CFG_DATA_BITS_8,
    		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
    };

    // Since all the LEDs are associated with the same port, &gpio0, we only need to check one of the devices.
	if (!device_is_ready(led0.port)){
		LOG_ERR("GPIO device is not ready\r\n");
		return 1;
	}

    if (!device_is_ready(uart)){
		LOG_ERR("UART device not ready\r\n");
		return 1 ;
	}

    ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1 ; 
	}
	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1 ;
	}
	ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1 ;
	}
    
    ret = uart_configure(uart, &uart_cfg); // Configure UART dynamically
	if (ret == -ENOSYS) {
		return -ENOSYS;
	}

    ret = uart_callback_set(uart, uart_cb, NULL);
	if (ret) {
		return 1;
	}

    ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_US);
	if (ret) {
		return 1;
	}

    ret = uart_rx_enable(uart ,rx_buf,sizeof rx_buf,RECEIVE_TIMEOUT);
	if (ret) {
		return 1;
	}

    while (1)
    {
        k_msleep(1);
    }
    

}