using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonScript : MonoBehaviour
{
    [SerializeField] SpriteRenderer Renderer;
    [SerializeField] GridManager grid;
    [SerializeField] private bool isClicked = false;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    public void OnMouseDown()
    {
        if (!isClicked)
        {
            isClicked = true;
        }
    }
    public void OnMouseExit()
    {
        isClicked = false;
    }
    public void OnMouseUp()
    {
        if (isClicked)
        {
            grid.Restart();
        }
    }
}
