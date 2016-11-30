/*
 * ASoC simple sound codec support
 *
 * Miroslav Rudisin <miero@seznam.cz>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>

#define BOTIC_CODEC_NAME          "botic-codec"
#define BOTIC_CODEC_DAI_NAME      "botic-hifi"
#define BOTIC_CODEC_PLAYBACK_NAME "Botic Playback"
#define BOTIC_CODEC_CAPTURE_NAME  "Botic Capture"

#define BOTIC_RATES (\
            SNDRV_PCM_RATE_CONTINUOUS | \
            SNDRV_PCM_RATE_11025 | \
            SNDRV_PCM_RATE_22050 | \
            SNDRV_PCM_RATE_44100 | \
            SNDRV_PCM_RATE_88200 | \
            SNDRV_PCM_RATE_176400 | \
            SNDRV_PCM_RATE_352800 | \
            SNDRV_PCM_RATE_705600 | \
            SNDRV_PCM_RATE_16000 | \
            SNDRV_PCM_RATE_32000 | \
            SNDRV_PCM_RATE_48000 | \
            SNDRV_PCM_RATE_96000 | \
            SNDRV_PCM_RATE_192000 | \
            SNDRV_PCM_RATE_384000 | \
            SNDRV_PCM_RATE_768000 | \
            0)

#define BOTIC_FORMATS (\
            SNDRV_PCM_FMTBIT_S16_LE | \
            SNDRV_PCM_FMTBIT_S24_3LE | \
            SNDRV_PCM_FMTBIT_S24_LE | \
            SNDRV_PCM_FMTBIT_S32_LE | \
            SNDRV_PCM_FMTBIT_DSD_U8 | \
            SNDRV_PCM_FMTBIT_DSD_U16_LE | \
            SNDRV_PCM_FMTBIT_DSD_U32_LE | \
            0)

static struct snd_soc_dai_driver botic_dai = {
    .name = BOTIC_CODEC_DAI_NAME,
    .playback = {
	.stream_name  = BOTIC_CODEC_PLAYBACK_NAME,
        .channels_min = 1,
        .channels_max = 8,
        .rate_min     = 11025,
        .rate_max     = 768000,
        .rates        = BOTIC_RATES,
        .formats      = BOTIC_FORMATS,
    },
    .capture = {
	.stream_name  = BOTIC_CODEC_CAPTURE_NAME,
        .channels_min = 1,
        .channels_max = 8,
        .rate_min     = 11025,
        .rate_max     = 768000,
        .rates        = BOTIC_RATES,
        .formats      = BOTIC_FORMATS,
    },
};

static int botic_component_probe(struct snd_soc_component *component)
{
    return 0;
}

static const struct snd_soc_dapm_route botic_dapm_routes[] = {
    {"AIF1TX", NULL, "Codec Input Pin1"},
    {"Codec Output Pin1", NULL, "AIF1RX"},
};

static const struct snd_soc_dapm_widget botic_dapm_widgets[] = {
    SND_SOC_DAPM_AIF_IN("AIF1RX", "Botic Codec Playback", 0,
                        SND_SOC_NOPM, 0, 0),
    SND_SOC_DAPM_AIF_OUT("AIF1TX", "Botic Codec Capture", 0,
                         SND_SOC_NOPM, 0, 0),
    SND_SOC_DAPM_INPUT("Codec Input Pin1"),
    SND_SOC_DAPM_OUTPUT("Codec Output Pin1"),
};

static struct snd_soc_component_driver soc_component_botic = {
    .probe                 = botic_component_probe,
    .dapm_widgets          = botic_dapm_widgets,
    .num_dapm_widgets      = ARRAY_SIZE(botic_dapm_widgets),
    .dapm_routes           = botic_dapm_routes,
    .num_dapm_routes       = ARRAY_SIZE(botic_dapm_routes),
    .idle_bias_on          = 1,
    .use_pmdown_time       = 1,
    .endianness            = 1,
    .non_legacy_dai_naming = 1,
};

static int asoc_botic_probe(struct platform_device *pdev)
{
    return snd_soc_register_component(&pdev->dev,
            &soc_component_botic, &botic_dai, 1);
}

static int asoc_botic_remove(struct platform_device *pdev)
{
    return 0;
}

#if defined(CONFIG_OF)
static const struct of_device_id asoc_botic_codec_dt_ids[] = {
    { .compatible = "botic-audio-codec" },
    { },
};

MODULE_DEVICE_TABLE(of, asoc_botic_codec_dt_ids);
#endif

static struct platform_driver asoc_botic_codec_driver = {
    .probe  = asoc_botic_probe,
    .remove = asoc_botic_remove,
    .driver = {
        .name = "asoc-botic-codec",
        .of_match_table = of_match_ptr(asoc_botic_codec_dt_ids),
    },
};

module_platform_driver(asoc_botic_codec_driver);

MODULE_AUTHOR("Miroslav Rudisin");
MODULE_DESCRIPTION("ASoC Botic sound codec");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:asoc-botic-codec");
