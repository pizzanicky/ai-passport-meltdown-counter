<p align="right"><a href="meltdown-counter.zh_CN.md">简体中文</a> · <strong>English</strong></p>

# Meltdown counter

This note records the behavior of the application. The public guide is the repository [README](../../README.md). The name is for the repository and the community page. The device does not show it as a title.

## Pages

Today shows the date, the speaker mark, today's count, and the controls. Up opens Statistics. Down opens the month. OK records one press.

Statistics comes before Today. It shows this week's seven bars, Monday through Sunday, and the change from yesterday. Down returns to Today. Another Up press stays here. OK records one press and returns to Today. Hold OK to mute without recording and without leaving the page.

The month is the current month only. Columns are weeks and rows run Monday through Sunday. Up returns to Today. Down stays. OK records one press and returns to Today. The page index is hidden while the mute mark flashes, so the two do not overlap.

There is no page wrap.

## Comparison and color

The comparison number has no plus or minus sign. A warm triangle means more than yesterday, a cool triangle means less, and 0 means the same. `--` means yesterday has no record. The count and the month squares share one color scale. A higher count uses a warmer color. Today has an extra outline. Days outside the month are empty, not zero.

The on-screen count uses `99999+` at the display limit. Storage keeps the full count.

## Sound

Sound is on by default. Counts 1 through 5 play the casing burst. Count 6 and above play the egg crack, and the new sound replaces the previous one. An unconfirmed-date press always uses the casing burst. A new Beijing day starts on the burst and does not play by itself. Mute records the press and plays nothing, including no catch-up later. Hold OK, page changes, time sync, and restoring history do not play the record sound.

Keep the egg-crack credit in [assets/README.md](../../assets/README.md) whenever firmware containing that sound is published.

## Date and saved presses

The calendar is Beijing time, with Monday as the first day of the week. The device does not trust a saved date after power-up. Wi-Fi runs only while the date is unconfirmed or setup is started again. After sync, the radio stops. The device name in the provisioning mini program is `BLUFI_FoloPassport`. The exact mini-program name is in the [Chinese guide](../../README.zh_CN.md).

Presses made before a trusted date stay aside. They are not filed into an old date, and they do not roll the month. When the date is known, add them to today or keep them separate. Filing them plays no record sound. Hold Up on the unconfirmed-date screen to clear the saved Wi-Fi network. Passwords are never logged.

A failed save shows a short on-screen notice and is retried. The new count is drawn before the save finishes. Power loss during the save can leave the previous record, so the count is durable only after the save completes.

## Out of scope

There is no timer, breathing exercise, ranking, mood label, or account. The community submission text and the checks still open are in the [release draft](../assets/community-release-draft.md).
