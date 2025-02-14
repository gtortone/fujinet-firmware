#ifndef NETWORK_H
#define NETWORK_H

#include <array>
#include <cstdint>
#include <esp_timer.h>
#include <memory>
#include <string>

#include "bus.h"
#include "fnjson.h"
#include "network_data.h"
#include "peoples_url_parser.h"
#include "Protocol.h"
#include "string_utils.h"

/**
 * The size of rx and tx buffers
 */
#define INPUT_BUFFER_SIZE 65535
#define OUTPUT_BUFFER_SIZE 65535
#define SPECIAL_BUFFER_SIZE 256

/**
 * The number of IEC secondary addresses (16)
 */
#define NUM_CHANNELS 16

using namespace std;

class iecNetwork : public virtualDevice
{    
public:

    /**
     * @brief CTOR
     */
    iecNetwork();

    /**
     * @brief DTOR
     */
    virtual ~iecNetwork();

    std::unordered_map<uint8_t, NetworkData> network_data_map;

    // /**
    //  * @brief the Receive buffers, for each channel
    //  */
    // string *receiveBuffer[NUM_CHANNELS];
    
    // /**
    //  * @brief the Transmit buffers, one for each channel.
    //  */
    // string *transmitBuffer[NUM_CHANNELS];

    // /**
    //  * @brief the Special buffers, one for each channel.
    //  */
    // string *specialBuffer[NUM_CHANNELS];

    // /**
    //  * @brief the protocol instance for given channel
    // */
    // NetworkProtocol *protocol[NUM_CHANNELS];

#if 0
    /**
     * @brief Process command fanned out from bus
     * @return new device state
     */
    device_state_t process() override;
#endif

    /**
     * @brief Check to see if SRQ needs to be asserted.
     * @param c Secondary channel # (0-15)
     */
    virtual void poll_interrupt(unsigned char c) override;

protected:
    virtual device_state_t openChannel(/*int chan, IECPayload &payload*/) override;
    virtual device_state_t closeChannel(/*int chan*/) override;
    virtual device_state_t readChannel(/*int chan*/) override;
    virtual device_state_t writeChannel(/*int chan, IECPayload &payload*/) override;

private:
    /**
     * @brief flag to indicate if the status result should be binary or string
     */
    bool is_binary_status = false;

    // /**
    //  * @brief the active URL for each channel
    //  */
    // string deviceSpec[NUM_CHANNELS];

    // /**
    //  * @brief the URL parser for each channel
    //  */
    // std::array<std::unique_ptr<PeoplesUrlParser>, NUM_CHANNELS> urlParser;

    // /**
    //  * @brief the prefix for each channel
    //  */
    // string prefix[NUM_CHANNELS];

    // /**
    //  * @brief the active Channel mode for each channel
    //  */
    // enum _channel_mode
    // {
    //     PROTOCOL,
    //     JSON
    // } channelMode[NUM_CHANNELS] =
    //     {
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL,
    //         PROTOCOL
    //     };

    // /**
    //  * @brief the active translation mode for each channel 
    //  */
    // uint8_t translationMode[NUM_CHANNELS];

    // /**
    //  * @brief the login (username) for each channel
    //  */
    // string login[NUM_CHANNELS];

    // /**
    //  * @brief the password for each channel
    //  */
    // string password[NUM_CHANNELS];

    // /**
    //  * @brief the JSON object for each channel
    //  */
    // FNJSON *json[NUM_CHANNELS];

    // /**
    //  * @brief # of bytes remaining in json query/channel
    //  */
    // int json_bytes_remaining[NUM_CHANNELS];

    /**
     * @brief signal file not found
     */
    bool file_not_found = false;

    /**
     * @brief active status channel
     */
    uint8_t active_status_channel=0;

    /**
     * @brief parse JSON
     */
    void parse_json();

    /**
     * @brief query JSON
     */
    void query_json();

    /**
     * @brief parse into bite size strings
     */
    void parse_bite();

    /**
     * @brief Set device ID from dos command
     */
    void set_device_id();

    /**
     * @brief Set channel to retrieve status from.
     */
    void set_status(bool is_binary);

    /**
     * @brief Set desired prefix for channel
     */
    void set_prefix();

    /**
     * @brief Get prefix for channel
     */
    void get_prefix();

    /**
     * @brief Set channel mode (e.g. protocol, or json)
     */
    void set_channel_mode();

    /**
     * @brief Set login/password
     */
    void set_login_password();

    /**
     * @brief Set translation mode
     */
    void set_translation_mode();

    /**
     * @brief ask protocol to perform idempotent filesystem operation
     * @param comnd the command to pass in cmdFrame
     */
    void fsop(unsigned char comnd);

    /**
     * @brief called to open a connection to a protocol
     */
    void iec_open();

    /**
     * @brief called to close a connection.
     */
    void iec_close();
    
    /**
     * @brief called when a TALK, then REOPEN happens on channel 0
     */
    void iec_reopen_load();

    /**
     * @brief called when TALK, then REOPEN happens on channel 1
     */
    void iec_reopen_save();

    /**
     * @brief called when REOPEN (to send/receive data)
     */
    void iec_reopen_channel();

    /**
     * @brief called when channel needs to listen for data from c=
     */
    void iec_reopen_channel_listen();

    /**
     * @brief called when channel needs to talk data to c=
     */
    void iec_reopen_channel_talk();

    /**
     * @brief called when LISTEN happens on command channel (15).
     */
    void iec_listen_command();

    /**
     * @brief called when TALK happens on command channel (15).
     */
    void iec_talk_command();

    /**
     * @brief called to process command either at open or listen
     */
    void iec_command();

    /**
     * @brief called to ask protocol to perform an operation with no payload
     */
    void perform_special_00();

    /**
     * @brief called to ask protocol to perform an operation with payload to computer (status)
     */
    void perform_special_40();

    /**
     * @brief called to ask protocol to perform an operation with no payload
     */
    void perform_special_80();

    /**
     * @brief process command for channel 0 (load)
     */
    void process_load();

    /**
     * @brief process command for channel 1 (save)
     */
    void process_save();

    /**
     * @brief process command channel
     */
    void process_command();

    /**
     * @brief process every other channel (2-14)
     */
    void process_channel();

    /**
     * @brief changes the open mode for the channel (e.g. to DELETE)
     */
    void set_open_params();

};

#endif /* NETWORK_H */
