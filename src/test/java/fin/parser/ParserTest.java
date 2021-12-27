package fin.parser;

import fin.model.AgentInstance;
import fin.model.Name;
import fin.model.NetDeclaration;
import org.hamcrest.Matcher;
import org.junit.Assert;
import org.junit.Test;

import static org.hamcrest.CoreMatchers.*;
import static org.junit.Assert.*;


public class ParserTest {
    @Test
    public void parse_sz() {
        NetDeclaration nd = Parser.parse("S(Z)");
        assertTrue(nd.getAgents().isEmpty());
        assertTrue(nd.getRules().isEmpty());
        assertThat(nd.getNet(), is(instanceOf(AgentInstance.class)));
        AgentInstance sI = (AgentInstance)nd.getNet();
        assertThat(sI.getName(), equalTo("S"));
        assertThat(sI.getConnections().get(0), is(instanceOf(Name.class)));
    }

    @Test
    public void parse_szp() {
        String nd =
                  "Z><plus(x,r) => x ~ r\n"
                + "S(x)><plus(y,r) => x = plus(y, m), r = S(m) \n" +
                "S(Z)";
    }
}
