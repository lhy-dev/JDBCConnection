import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;
import java.sql.ResultSetMetaData;

public class DevJDBC {

	static Connection conn;
	static PreparedStatement stmt;
	static ResultSet result;
	static Map<Integer, String> items= new HashMap<Integer,String>() ;
	static Map<String, Double> values = new HashMap<String, Double>();
	static List<PreparedStatement> stmtlist = new ArrayList<PreparedStatement>();
	//static List<HashMap<Integer, String>> itemlist = new ArrayList<HashMap<Integer,String>>();

	public static boolean getConnection(String url, String user, String password) {

		try {

			Class.forName("oracle.jdbc.driver.OracleDriver");
			System.out.println("connecting the database...");
			conn = DriverManager.getConnection(url, user, password);
		} catch (Exception e) {
			e.printStackTrace();
		}

		if (conn != null) {
			System.out.println("connect the database success");
			return true;

		} else {
			return false;
		}
	}

	public static int prepareStmt(String sql) {
		int index = 0 ;
		try {
			// System.out.println(sql);
			stmt = conn.prepareStatement(sql);
			stmtlist.add(stmt);
		} catch (Exception e) {
			e.printStackTrace();
		}
		if (stmt != null) {
			System.out.println(sql);
			index = stmtlist.size();
			System.out.println("prepare sql stmt count:" + index);
			return index;
		} else {

			return 0;

		}

	}

	public static boolean executeQuery(int index) {
		// Tag_data Tag = new Tag_data();
		try {
			if (stmt != null) {
				System.out.println("index of stmt:" + (index-1));
				PreparedStatement execstmt = stmtlist.get(index-1);
				result = execstmt.executeQuery();
			}
		} catch (Exception e) {

			e.printStackTrace();
		}
		if (result != null) {
			System.out.println("execute query success");
			return true;
		} else {
			System.out.println("execute query failed");
			return false;
		}
	}

	public static void setColname(int index, String colname) {
		
		items.put(index, colname);
		System.out.println(items);
		//itemlist.add(items);
		//System.out.println(itemlist);
	}

	public static double readValue(int i ) {
		double value = 0;
		int j;
		try {
			ResultSetMetaData rsmd = result.getMetaData();
			if (items.containsKey( i )) {
				String colname = items.get(i);
				String colname1 = colname.toUpperCase();
				while (result.next()) {
					for (j = 1; j < rsmd.getColumnCount() + 1; j++)
					{
						// System.out.println("put the value start..");
						// value[j] = result.getDouble(j);
						values.put(rsmd.getColumnName(j), result.getDouble(j));
						System.out.println("database value: " + " columnname:" + rsmd.getColumnName(j) + " value:"
								+ result.getDouble(j));
					}
				}
				if (values.containsKey(colname1)) {
					value = values.get(colname1);
					System.out.println(" read  tag value:" + " columnname:" + colname1 + "  tag value:" + value);
				}
			}
		} catch (Exception e) {

			e.printStackTrace();
		}
		return value;
	}
	
	public static void logoff() throws SQLException {

		if (result != null)
			result.close();
		if (stmtlist != null)
			{     for(PreparedStatement stmt:stmtlist ){
		        	stmt.close();
				}
			}
		if (conn != null)
			conn.close();
		items.clear();
		values.clear();
		stmtlist.clear();
		//itemlist.clear();
		System.out.println("database closed!");

	}

}
