<%@ page contentType="text/html;charset=gb2312"%> 
<%@ page language="java" %>
<%@ page import="javax.sql.DataSource"%> 
<%@ page import="javax.naming.Context"%> 
<%@ page import="javax.naming.InitialContext"%> 
<%@ page import="java.sql.*"%> 

<html>
	<head>
	<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
	<title>JSP-SQL���ݿ��ѯ</title>
	</head>
	<body>
	
  
<h2>��ѯ�����</h2>
<%
String ls=null;
DataSource ds=null;
try{
Context initContext = new InitialContext();
Context envContext  = (Context)initContext.lookup("java:/comp/env");
ds = (DataSource)envContext.lookup("jdbc/testDB");
Connection conn = ds.getConnection();
Statement stmt=conn.createStatement();   
String sql="select * from test_list ";   
ResultSet rs=stmt.executeQuery(sql);

while(rs.next())
{
%>
������<%=rs.getString(1)%>
���ţ�<%=rs.getString(2)%>
<br>

<%
}
rs.close(); 
stmt.close(); 
conn.close(); 
}
catch(SQLException e)
{
out.println(e);
}
%> 
	</body>
</html>
