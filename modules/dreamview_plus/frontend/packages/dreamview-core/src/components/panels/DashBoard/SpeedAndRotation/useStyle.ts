import { useMakeStyle } from '@dreamview/dreamview-theme';

export default function useStyle() {
    const hoc = useMakeStyle((theme) => ({
        'dash-board-speed-rotation': {
            display: 'flex',
            flexDirection: 'row',
            flex: '4.625',
            columnGap: '10px',
        },
        error: {
            color: theme.tokens.colors.error2,
        },
        info: {
            color: theme.tokens.colors.brand3,
        },
        warn: {
            color: theme.tokens.colors.warn2,
        },
    }));
    return hoc();
}
