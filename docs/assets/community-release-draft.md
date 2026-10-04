<p align="right"><a href="community-release-draft.zh_CN.md">简体中文</a> · <strong>English</strong></p>

# Community release draft

Status: prepared locally. This draft is not an upload approval. Do not submit it until the author reviews every field and completes the open checks below.

| Field | Prepared value |
| --- | --- |
| Chinese title | See the exact title in the [Chinese draft](community-release-draft.zh_CN.md) |
| English title | Meltdown Counter |
| Cover | [`../../assets/images/community-cover.png`](../../assets/images/community-cover.png), PNG, 1152 x 1536. A host composition of the three interface renders with example data. It is not a photograph of a device. |
| Public source | `https://github.com/pizzanicky/ai-passport-meltdown-counter` |
| Firmware | Not packed from this commit. Before upload, run `./tools/validate.sh --firmware` on this revision and copy the merged image at `build/FoloToy-AI-Passport-full.bin`, with its full image, app, and ELF hashes. Do not substitute an app-only image. |

## English description

Press OK when a moment feels like too much. Today shows the count, Statistics shows the week and yesterday, and the month uses color for each day. Sound stays on until you hold OK. The calendar uses Beijing time and starts the week on Monday.

## English instructions

Press OK once to record the moment. Today shows the count and plays a short sound. Press Up for Statistics, which shows this week's seven bars from Monday through Sunday and the change from yesterday. Press Down there to return. On Today, press Down for the month, then Up to return. The pages do not wrap. A short OK press on Statistics or the month also records one count and returns to Today.

A warm triangle means more than yesterday, a cool triangle means less, and the number is the size of the change. The same count shows 0. If yesterday has no record, the comparison shows `--`. The month is the current month. A higher count uses a warmer color. Today has an extra outline, and days outside the month are blank.

Hold OK to mute or unmute. The speaker mark shows the state. Mute still records the press and plays nothing. Counts 1 through 5 play a casing burst. From 6, the press plays an egg crack. A new day starts on the burst again and does not play by itself.

Until this boot confirms the date, the screen says the date is unconfirmed. Open the provisioning mini program named in the Chinese instructions, select `BLUFI_FoloPassport`, and provide a 2.4 GHz Wi-Fi network. After the time syncs, the radio turns off. A shown time is trusted only after this boot finishes syncing. Hold Up on that screen to clear the saved Wi-Fi network and start setup again.

Presses made before the date is known stay aside and use the casing burst. When the date is known, add them to today or keep them separate. Filing them plays no record sound. The on-screen count stops at `99999+`. There is no timer, breathing exercise, ranking, mood label, or account.

## Checks before publication

- Rebuild `./tools/validate.sh --firmware` on the public commit and record the merged image, app, and ELF hashes. Recheck after any source change.
- Confirm the cover is a 1152 x 1536 PNG at most 10 MiB, and inspect the file that will be uploaded. Keep its caption as a host illustration with example data, not a device photograph.
- On the device, accept the three pages, the count, mute, both sounds, the month colors, and a full provisioning and time-sync pass. A merged image written from address 0 can replace saved records and Wi-Fi. Explain that before asking for flash consent.
- Review every field with the author. Authorization on the official site, and a separate yes to upload, are still required. This draft does not authorize either.
