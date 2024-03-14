#include <psdcli_extract.h>

INIT_PSD_RESULT;

psd_result_t extract_image_p(const psd_header_t *const header,
                             psd_cursor_t *const cursor)
{
        psd_skip_color_mode_data_section(cursor);
        psd_skip_image_resources_section(cursor);
        psd_skip_layer_and_mask_information_section(cursor);

        psd_up_error(psd_read_image_data(header, cursor));
        return_psd_success(psd_result__.pointer);
}

psd_result_t extract_image_pp(const psd_header_t *const header,
                              psd_cursor_t *const cursor)
{
        psd_byte_t *bufferp;
        psd_byte_t **bufferpp;

        psd_channels_t channels;
        psd_width_t width;
        psd_height_t height;

        channels = header->channels;
        width = header->width;
        height = header->height;

        psd_up_error(extract_image_p(header, cursor));
        bufferp = psd_result__.pointer;
        bufferpp = malloc(height * sizeof(psd_byte_t *));

        for (size_t i = 0, n = 0; i < height; ++i) {
                bufferpp[i] = malloc(width * channels);

                for (size_t j = 0; j < width * channels; ++j, ++n) {
                        bufferpp[i][j] = bufferp[n];
                }
        }

        free(bufferp);
        return_psd_success(bufferpp);
}
