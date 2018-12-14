#ifdef __cplusplus
extern "C" {
#endif

/*
 * Closes the sd and deletes the `struct mgos_sd*`
 */
//void mgos_sd_close();

/*
 * Prints information about the connected SD card
 */
//void mgos_sd_print_info(struct json_out *out);

/*
 * Returns the mount point of the SD card.
 */
///const char *mgos_sd_get_mount_point();

/*
 * Lists the contents of the SD card.
 */
//bool mgos_sd_list(const char *path, struct json_out *out);


bool mgos_gps_init(void);


#ifdef __cplusplus
}
#endif



