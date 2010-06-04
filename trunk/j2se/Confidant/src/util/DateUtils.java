package util;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;

public class DateUtils {


    public static java.util.Date stringToDate(String strDate, String pattern) {
        if (strDate == null || strDate.trim().length() <= 0)
            return null;
        try {
            SimpleDateFormat sdf = new SimpleDateFormat(pattern);
            return sdf.parse(strDate);
        } catch (ParseException e) {
            return null;
        }
    }

    /*
     * public static String dateToString(java.util.Date date) { if (date ==
     * null) return null; SimpleDateFormat sdf = new
     * SimpleDateFormat(defaultPattern); return sdf.format(date); }
     */

    public static String dateToString(java.util.Date date, String pattern) {
        if (date == null)
            return null;
        SimpleDateFormat sdf = new SimpleDateFormat(pattern);
        return sdf.format(date);
    }

    public static Date dateAddMinutes(Date date, int minutes) {
        if (date == null)
            return null;
        Calendar cal = Calendar.getInstance();
        cal.setTime(date);
        cal.add(Calendar.MINUTE, minutes);
        return cal.getTime();
    }

    public static Date dateAddHours(Date date, int hours) {
        if (date == null)
            return null;
        Calendar cal = Calendar.getInstance();
        cal.setTime(date);
        cal.add(Calendar.HOUR, hours);
        return cal.getTime();
    }

    public static Date dateAddDays(Date date, int addDays) {
        if (date == null)
            return null;
        Calendar cal = Calendar.getInstance();
        cal.setTime(date);
        cal.add(Calendar.DATE, addDays);
        return cal.getTime();
    }

    public static Date dateAddMonths(Date date, int addMonths) {
        if (date == null)
            return null;
        Calendar cal = Calendar.getInstance();
        cal.setTime(date);
        cal.add(Calendar.MONTH, addMonths);
        return cal.getTime();
    }

    public static Date dateAddMonths(Date date, double months) {
        if (date == null)
            return null;
        int addMonths = (int) Math.ceil(months);
        int addDays = (int) Math.round((months - addMonths) * 31);
        Calendar cal = Calendar.getInstance();
        cal.setTime(date);
        cal.add(Calendar.MONTH, addMonths);
        cal.add(Calendar.DATE, addDays);
        return cal.getTime();
    }

    public static Date dateAddYears(Date date, int addYears) {
        if (date == null)
            return null;
        Calendar cal = Calendar.getInstance();
        cal.setTime(date);
        cal.add(Calendar.YEAR, addYears);
        return cal.getTime();
    }

    public static boolean isDate(String strDate, String pattern) {
        if (strDate == null || strDate.trim().length() <= 0)
            return false;
        try {
            SimpleDateFormat sdf = new SimpleDateFormat(pattern);
            sdf.parse(strDate);
            return true;
        } catch (ParseException e) {
            return false;
        }
    }

    public static int getIntervalOfDay(Date d1, Date d2) {
        int elapsed = 0;
        boolean isAfter = false;
        Date temp;
        if (d1.after(d2)) {
            temp = d1;
            d1 = d2;
            d2 = temp;
            isAfter = true;
        }

        while (d1.before(d2)) {
            d1 = dateAddDays(d1, 1);
            elapsed++;
        }
        if (isAfter)
            elapsed = 0 - elapsed;
        return elapsed;
    }

    /**
     * Elapsed days based on current time
     * 
     * @param date
     *            Date
     * 
     * @return int number of days
     */
    public static int getElapsedDays(Date date) {
        return elapsed(date, Calendar.DATE);
    }

    /**
     * Elapsed days based on two Date objects
     * 
     * @param d1
     *            Date
     * @param d2
     *            Date
     * 
     * @return int number of days
     */
    public static int getElapsedDays(Date d1, Date d2) {
        return elapsed(d1, d2, Calendar.DATE);
    }

    /**
     * Elapsed months based on current time
     * 
     * @param date
     *            Date
     * 
     * @return int number of months
     */
    public static int getElapsedMonths(Date date) {
        return elapsed(date, Calendar.MONTH);
    }

    /**
     * Elapsed months based on two Date objects
     * 
     * @param d1
     *            Date
     * @param d2
     *            Date
     * 
     * @return int number of months
     */
    public static int getElapsedMonths(Date d1, Date d2) {
        return elapsed(d1, d2, Calendar.MONTH);
    }

    /**
     * Elapsed years based on current time
     * 
     * @param date
     *            Date
     * 
     * @return int number of years
     */
    public static int getElapsedYears(Date date) {
        return elapsed(date, Calendar.YEAR);
    }

    /**
     * Elapsed years based on two Date objects
     * 
     * @param d1
     *            Date
     * @param d2
     *            Date
     * 
     * @return int number of years
     */
    public static int getElapsedYears(Date d1, Date d2) {
        return elapsed(d1, d2, Calendar.YEAR);
    }

    /**
     * All elaspsed types
     * 
     * @param g1
     *            GregorianCalendar
     * @param g2
     *            GregorianCalendar
     * @param type
     *            int (Calendar.FIELD_NAME)
     * 
     * @return int number of elapsed "type"
     */
    private static int elapsed(GregorianCalendar g1, GregorianCalendar g2, int type) {
        GregorianCalendar gc1, gc2;
        int elapsed = 0;
        // Create copies since we will be clearing/adding
        if (g2.after(g1)) {
            gc2 = (GregorianCalendar) g2.clone();
            gc1 = (GregorianCalendar) g1.clone();
        } else {
            gc2 = (GregorianCalendar) g1.clone();
            gc1 = (GregorianCalendar) g2.clone();
        }
        if (type == Calendar.MONTH || type == Calendar.YEAR) {
            gc1.clear(Calendar.DATE);
            gc2.clear(Calendar.DATE);
        }
        if (type == Calendar.YEAR) {
            gc1.clear(Calendar.MONTH);
            gc2.clear(Calendar.MONTH);
        }
        while (gc1.before(gc2)) {
            gc1.add(type, 1);
            elapsed++;
        }
        return elapsed;
    }

    /**
     * All elaspsed types based on date and current Date
     * 
     * @param date
     *            Date
     * @param type
     *            int (Calendar.FIELD_NAME)
     * 
     * @return int number of elapsed "type"
     */
    private static int elapsed(Date date, int type) {
        return elapsed(date, new Date(), type);
    }

    /**
     * All elaspsed types
     * 
     * @param d1
     *            Date
     * @param d2
     *            Date
     * @param type
     *            int (Calendar.FIELD_NAME)
     * 
     * @return int number of elapsed "type"
     */
    private static int elapsed(Date d1, Date d2, int type) {
        Calendar cal = Calendar.getInstance();
        cal.setTime(d1);
        GregorianCalendar g1 = new GregorianCalendar(cal.get(Calendar.YEAR), cal.get(Calendar.MONTH), cal.get(Calendar.DATE));
        cal.setTime(d2);
        GregorianCalendar g2 = new GregorianCalendar(cal.get(Calendar.YEAR), cal.get(Calendar.MONTH), cal.get(Calendar.DATE));
        return elapsed(g1, g2, type);
    }
    
    public static int getDaysOfMonth(Date date){
        Calendar cal = Calendar.getInstance();
        cal.setTime(date);
        return getMonthDateCount(cal);
    }
    
    /**
     * 得到指定月份的天数
     * 
     * @param cal
     * @return
     */
    protected static int getMonthDateCount(Calendar cal) {
        Calendar cal2 = (Calendar) cal.clone();
        cal2.add(Calendar.MONTH, 1);
        cal2.set(Calendar.DAY_OF_MONTH, 1);
        cal2.add(Calendar.DAY_OF_MONTH, -1);
        return cal2.get(Calendar.DAY_OF_MONTH);
    }
}
