<p align="right"><a href="community-release-draft.zh_CN.md">简体中文</a> · <strong>English</strong></p>

# Community release draft

Status: submitted for review on 2026-10-04. Project 933, revision 1951, slug `community-bd87a35c`. The review status is pending. It is not public yet.

| Field | Prepared value |
| --- | --- |
| Chinese title | See the exact title in the [Chinese draft](community-release-draft.zh_CN.md) |
| English title | Meltdown Counter |
| Cover | [`../../assets/images/community-cover.png`](../../assets/images/community-cover.png), PNG, 1152 x 1536. A host composition of the three interface renders with example data. It is not a photograph of a device. |
| Public source | `https://github.com/pizzanicky/ai-passport-meltdown-counter` |
| Firmware | Merged image `build/FoloToy-AI-Passport-full.bin` packed from clean public commit `aac8752` before this note. Version string `aac8752`. Merged SHA-256 `f5ffcfc61fc819c806614997abbbdfc4a8ff5cd3cc54af7ca9c47744c15548b4`, 1764512 bytes, written from address 0. App SHA-256 `bf74aa242a3665a15143f60117703925f47b0fedb9a62e2301ecbcda6d55a62e`, 1698976 bytes. ELF SHA-256 `fea96c2aec1fed0dd17dbe4b2e7a9104d58b9f3ac3e9e50faae1fe8fd16d5666`. |

## English description

Press OK when a moment feels like too much. Today shows the count, Statistics shows the week and yesterday, and the month uses color for each day. Sound stays on until you hold OK. The calendar uses Beijing time and starts the week on Monday.

## English instructions

Press OK once to record the moment. Today shows the count and plays a short sound. Press Up for Statistics, which shows this week's seven bars from Monday through Sunday and the change from yesterday. Press Down there to return. On Today, press Down for the month, then Up to return. The pages do not wrap. Page hints are centered. The month puts its return hint at the top and shows no page index. Today shows the battery percent beside the speaker. After one minute without a button, the backlight turns off, and the next button only turns it back on. A short OK press on Statistics or the month also records one count and returns to Today.

A warm triangle means more than yesterday, a cool triangle means less, and the number is the size of the change. The same count shows 0. If yesterday has no record, the comparison shows `--`. The month is the current month. A higher count uses a deeper red. Today has an extra outline, and days outside the month are blank.

Hold OK to mute or unmute. The speaker mark shows the state. Mute still records the press and plays nothing. Counts 1 through 5 play a casing burst. From 6, the press plays an egg crack. A new day starts on the burst again and does not play by itself.

Until this boot confirms the date, the screen says the date is unconfirmed. Open the provisioning mini program named in the Chinese instructions, select `BLUFI_FoloPassport`, and provide a 2.4 GHz Wi-Fi network. After the time syncs, the radio turns off. A shown time is trusted only after this boot finishes syncing. Hold Up on that screen to clear the saved Wi-Fi network and start setup again.

Presses made before the date is known stay aside and use the casing burst. When the date is known, add them to today or keep them separate. Filing them plays no record sound. The on-screen count stops at `99999+`. There is no timer, breathing exercise, ranking, mood label, or account.

## Checks before publication

- `./tools/validate.sh --firmware` passed on clean public commit `aac8752`. Hashes are in the table above. Rebuild after any source change.
- On 2026-10-04 the live creator-selectable tags were only multiplayer and family. Neither applies, so the submission omits them. The platform assigns the category.
- Confirm the cover is a 1152 x 1536 PNG at most 10 MiB, and inspect the file that will be uploaded. Keep its caption as a host illustration with example data, not a device photograph.
- On 2026-10-04 the author accepted the latest on-device version: three pages, the count, mute, both sounds, battery, and the one-minute blank. A merged image written from address 0 can replace saved records and Wi-Fi.
- Submitted for review as project 933, revision 1951. Pending review is not a public release.
