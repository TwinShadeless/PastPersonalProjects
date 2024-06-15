using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraScript : MonoBehaviour
{
    [SerializeField] private Camera camObject;
    [SerializeField] private float sensitivity;
    [SerializeField] private GridManager grid;
    [SerializeField] private ButtonScript button;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if ((Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.LeftArrow)) && camObject.transform.position.x > 0)
        {
            camObject.transform.Translate(sensitivity * -1, 0, 0);
        }
        if ((Input.GetKey(KeyCode.W) || Input.GetKey(KeyCode.UpArrow)) && camObject.transform.position.y < grid.GetHeight())
        {
            camObject.transform.Translate(0, sensitivity, 0);
        }
        if ((Input.GetKey(KeyCode.D) || Input.GetKey(KeyCode.RightArrow)) && camObject.transform.position.x < grid.GetWidth())
        {
            camObject.transform.Translate(sensitivity, 0, 0);
        }
        if ((Input.GetKey(KeyCode.S) || Input.GetKey(KeyCode.DownArrow)) && camObject.transform.position.y > 0)
        {
            camObject.transform.Translate(0, sensitivity * -1, 0);
        }
    }
}
