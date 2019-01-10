/**
  ******************************************************************************
  * @file   main.c
  * @author Jordan Goulder
  * @brief  Read the User button on the NUCLEO board.
  ******************************************************************************
*/

#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"

// LED delay times
#define LED_LONG_DELAY  (500)
#define LED_SHORT_DELAY (100)

// Button delay to debounce button presses
#define BUTTON_DELAY    (25)

// Button states
#define BUTTON_PRESSED  (0)
#define BUTTON_RELEASED (1)


int main(void)
{
    // Initialize the Hardware Abstraction Library
    HAL_Init();

    // Initialize LED2 on the NUCLEO board
    BSP_LED_Init(LED2);

    // Start with a long delay between LED toggles
    uint32_t ledToggleDelay = LED_LONG_DELAY;

    // Set the next time to toggle the LED
    uint32_t ledNextToggle = HAL_GetTick() + ledToggleDelay;

    // Initialize the User button on the NUCLEO board
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

    // Set the next time to check the button
    uint32_t buttonNextCheck = HAL_GetTick();

    // Initialize the last button state
    uint32_t buttonLastState = BSP_PB_GetState(BUTTON_USER);

    // Blink LED2 and change the toggle delay on button presses
    for(;;)
    {
        // Get the current time
        uint32_t now = HAL_GetTick();

        // Is it time to toggle the LED?
        if (now >= ledNextToggle)
        {
            // Toggle LED2
            BSP_LED_Toggle(LED2);

            // Set the next time to toggle the LED
            ledNextToggle = now + ledToggleDelay;
        }

        // Is it time to check the button?
        if (now >= buttonNextCheck)
        {
            // Get the current button state
            uint32_t buttonState = BSP_PB_GetState(BUTTON_USER);

            // Check to see if the button was just pressed
            if ((buttonState != buttonLastState) && (buttonState == BUTTON_PRESSED))
            {
                // Change the LED toggle delay
                if (ledToggleDelay == LED_LONG_DELAY)
                {
                    ledToggleDelay = LED_SHORT_DELAY;
                }
                else
                {
                    ledToggleDelay = LED_LONG_DELAY;
                }

                // Go ahead and start toggling the LED with the new time
                ledNextToggle = now;
            }

            // Save the last button state
            buttonLastState = buttonState;

            // Set the next time to check the button
            buttonNextCheck = now + BUTTON_DELAY;
        }
    }
}
