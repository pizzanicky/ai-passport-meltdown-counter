<p align="right"><a href="README.zh_CN.md">简体中文</a> · <strong>English</strong></p>

# Meltdown Counter

Press OK once when a moment feels like too much. The device records it, plays a short sound, and shows today's count. Statistics shows the week, and the month shows every day so far.

On Today, press Up for Statistics and Down for the month. On Statistics, press Down to return to Today. On the month, press Up to return to Today. The pages do not wrap. A short OK press on Statistics or the month also records one count and returns to Today.

Statistics draws seven bars for this week, Monday through Sunday, and the change from yesterday. A warm triangle means the count went up, a cool triangle means it went down, and the number is the size of that change. A flat day shows 0. If yesterday has no record, the comparison shows `--`.

The month page is the current calendar month. Each day is a square, and a higher count uses a warmer color. Today has an extra outline. Days outside this month are blank.

Hold OK to mute or unmute. The speaker mark shows whether sound is on. Mute still records the press and plays nothing. The choice is kept.

Counts from 1 to 5 play a short casing burst. From 6 onward, the press plays an egg crack and replaces any sound still playing. A new day starts on the burst again and does not play by itself. Holding OK, changing pages, syncing the clock, and restoring a saved record stay silent.

The calendar uses Beijing time, and the week starts on Monday. Until this boot confirms the date, the screen says the date is unconfirmed. A short OK press is kept aside and uses the casing burst. It is not placed on a day until the date is known. If presses are still waiting, add them to today or keep them separate. Hold Up on the unconfirmed-date screen to forget the saved Wi-Fi network and start setup again.

For the first clock setup, open the provisioning mini program named in the [Chinese instructions](README.zh_CN.md), select `BLUFI_FoloPassport`, and provide a 2.4 GHz Wi-Fi network. After the time syncs, the radio turns off. A time on screen is trusted only after this boot finishes syncing.

The on-screen count stops at `99999+`. The stored count does not stop there. There is no timer, breathing exercise, ranking, mood label, or account.

Sound credits for the egg crack are in [assets/README.md](assets/README.md). Prepared community fields and the checks still open are in the [release draft](docs/assets/community-release-draft.md).
